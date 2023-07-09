/*********************************************************************************
 Copyright 2017 GlobalPlatform, Inc.

 Licensed under the GlobalPlatform/Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 https://github.com/GlobalPlatform/SE-test-IP-connector/blob/master/Charter%20and%20Rules%20for%20the%20SE%20IP%20connector.docx

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 *********************************************************************************/

#include "client/client_engine.hpp"
#include "client/client_tcp_socket.hpp"
#include "config/config_wrapper.hpp"
#include "constants/default_values.hpp"
#include "constants/request_code.hpp"
#include "constants/response_packet.hpp"
#include "logger/logger.hpp"
#include "terminal/factories/factory.hpp"
#include "terminal/flyweight_terminal_factory.hpp"
#include "terminal/terminals/terminal.hpp"
#include "terminal/terminals/utils/type_converter.hpp"
#include "nlohmann/json.hpp"
#include "plog/include/plog/Log.h"
#include "plog/include/plog/Appenders/ColorConsoleAppender.h"
#include "plog/include/plog/Appenders/RollingFileAppender.h"

#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

namespace client {

ResponsePacket ClientEngine::initClient(std::string path, FlyweightTerminalFactory available_terminals, FlyweightRequests available_requests) {
	if ((path.size() > 1) && (path.at(0) == '{'))
	{
		config_.initFromJson(path);
	}
	else
	{
		config_.init(path);
	}

	ITerminalFactory* terminal_factory =  available_terminals.getFactory(config_.getValue("terminal"));
	if (terminal_factory == NULL) {
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_INVALID_TERMINAL, .err_client_description = "Failed to initialize the client: terminal not found" };
		return response_packet;
	}

	terminal_ = terminal_factory->create();
	requests_ = available_requests;
	socket_ = new ClientTCPSocket();
	logger::setup(&config_);

	// launch terminal
	ResponsePacket response_packet;
	response_packet = terminal_->init();
	if (response_packet.err_terminal_code != SUCCESS || response_packet.err_card_code != SUCCESS) {
		LOG_INFO << "Client unable to be initialized";
		return response_packet;
	}
	initialized_ = true;

	LOG_INFO << "Client initialized successfully";
	return response_packet;
}

ResponsePacket ClientEngine::loadAndListReaders() {
	ResponsePacket response;
	if (!initialized_.load()) {
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_INVALID_STATE, .err_client_description = "Client must be initialized correctly" };
		return response_packet;
	}
	return terminal_->loadAndListReaders();
}

ResponsePacket ClientEngine::connectClient(const char* reader, const char* ip, const char* port) {
	ResponsePacket packet;
	if (!initialized_.load()) {
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_INVALID_STATE, .err_client_description = "Failed to connect: client must be initialized correctly" };
		return response_packet;
	}

	if (connected_.load()) {
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_INVALID_STATE, .err_client_description = "Failed to connect: client is already connected" };
		return response_packet;
	}

	// init socket
	LOG_INFO << "Client trying to connect on IP " << ip << " port " << port;
	if (!socket_->initClient(ip, port)) {
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_NETWORK, .err_client_description = "Failed to connect: initialization failed" };
		return response_packet;
	}

	// connect to the server
	if (!socket_->connectClient()) {
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_NETWORK, .err_client_description = "Failed to connect: check the server" };
		return response_packet;
	}

	// connect to the terminal
	packet = terminal_->connect(reader);
	if (packet.err_card_code < 0 || packet.err_terminal_code < 0) {
		socket_ ->closeClient();
		return packet;
	}

	// perform handshake procedure
	std::string name = config_.getValue("name", DEFAULT_NAME).append(" - ").append(reader);
	if (!socket_->sendPacket(name.c_str())) {
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_NETWORK, .err_client_description = "Failed to connect: failed to perform handshake" };
		return response_packet;
	}

	connected_ = true;
	LOG_INFO << "Client connected on IP " << ip << " port " << port;

	// start waiting for requests on a different thread
	std::thread thr(&ClientEngine::waitingRequests, this);
	thr.detach();

	return packet;
}

ResponsePacket ClientEngine::disconnectClient() {
	if (!connected_.load()) {
		LOG_DEBUG << "Failed to disconnect: not connected yet";
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_INVALID_STATE, .err_client_description = "Failed to disconnect: not connected yet" };
		return response_packet;
	}

	connected_ = false;
	socket_->closeClient();
	ResponsePacket response = terminal_->disconnect();
	if (notifyConnectionLost_ != 0) {
		notifyConnectionLost_("End of connection");
	}

	LOG_INFO << "Client disconnected successfully";
	return response;
}

ResponsePacket ClientEngine::waitingRequests() {
	char request[DEFAULT_BUFLEN];
	bool response;

	LOG_INFO << "Client ready to process incoming requests";

	// receives until the server closes the connection
	while (connected_.load()) {
		response = socket_->receivePacket(request);
		if (response) {
			handleRequest(request);
		} else {
			disconnectClient();
		}
	}

	LOG_INFO << "Client not waiting for requests";

	ResponsePacket response_packet;
	return response_packet;
}

ResponsePacket ClientEngine::handleRequest(std::string request) {
	nlohmann::json jrequest;
	nlohmann::json jresponse;
	ResponsePacket response_packet;

	LOG_INFO << "Request received from server: " << request;
	if (notifyRequestReceived_ != 0) {
		notifyRequestReceived_(request.c_str());
	}

	// build the request using json
	try {
		jrequest = nlohmann::json::parse(request);
	} catch (json::parse_error &err) {
		LOG_DEBUG << "Error while parsing the request [request:" << request << "]";
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_JSON_PARSING, .err_client_description = "Error while parsing the request" };
		jresponse = response_packet;
		return sendResult(jresponse.dump());
	}

	unsigned long int length;
	unsigned char* command = utils::stringToUnsignedChar(jrequest["data"].get<std::string>(), &length);

	// retrieve the request handler
	IRequest* request_handler = requests_.getRequest(jrequest["request"]);

	if (request_handler == NULL) {
		LOG_DEBUG << "The request doesn't exist [request:" << request << "]";
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_INVALID_REQUEST, .err_client_description = "The request doesn't exist" };
		jresponse = response_packet;
		return sendResult(jresponse.dump());
	}

	// launch a thread to perform the request
	auto future = std::async(std::launch::async, &IRequest::run, request_handler, terminal_, this, command, length);
	// block until the timeout has elapsed or the result becomes available
	if (future.wait_for(std::chrono::milliseconds(jrequest["timeout"])) == std::future_status::timeout) {
		LOG_DEBUG << "Response time from terminal has elapsed [request:" << request << "]";
		pending_futures_.push_back(std::move(future));
		for (long long unsigned int i = 0; i < pending_futures_.size(); i++) {
			if (pending_futures_[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
				pending_futures_.erase(pending_futures_.begin() + i);
			}
		}
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_TIMEOUT, .err_client_description = "Response time from terminal has elapsed" };
		jresponse = response_packet;
	} else {
		jresponse = future.get();
	}
	return sendResult(jresponse.dump());
}

ResponsePacket ClientEngine::sendResult(std::string result) {
	if (!socket_->sendPacket(result.c_str())) {
		LOG_DEBUG << "Error during sendResult";
		ResponsePacket response_packet = { .response = "KO", .err_client_code = ERR_NETWORK, .err_client_description = "Network error on send response" };
		return response_packet;
	}

	LOG_INFO << "Response sent to server: " << result;
	if (notifyResponseSent_ != 0) {
		notifyResponseSent_(result.c_str());
	}

	ResponsePacket response_packet;
	return response_packet;
}

} /* namespace client */
