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

#ifndef CLIENT_ENGINE_HPP_
#define CLIENT_ENGINE_HPP_

#include "client/client_tcp_socket.hpp"
#include "client/requests/flyweight_requests.hpp"
#include "constants/callback.hpp"
#include "config/config_wrapper.hpp"
#include "constants/response_packet.hpp"
#include "terminal/flyweight_terminal_factory.hpp"
#include "terminal/terminals/terminal.hpp"

#include <atomic>
#include <future>
#include <thread>

namespace client {

class ClientEngine {
private:
	ConfigWrapper& config_ = ConfigWrapper::getInstance();
	ClientTCPSocket* socket_;
	ITerminalLayer* terminal_;
	std::thread requests_thread_;
	std::vector<std::future<ResponsePacket>> pending_futures_;
	std::atomic<bool> connected_ { false };
	std::atomic<bool> initialized_ { false };
	FlyweightRequests requests_;
	Callback notifyConnectionLost_, notifyRequestReceived_, notifyResponseSent_;
public:
	ClientEngine(Callback notifyConnectionLost, Callback notifyRequestReceived, Callback notifyResponseSent) {
		this->notifyConnectionLost_ = notifyConnectionLost;
		this->notifyRequestReceived_ = notifyRequestReceived;
		this->notifyResponseSent_ = notifyResponseSent;
	}

	~ClientEngine() {
		delete terminal_;
		delete socket_;
	}

	/**
	 * initClient - initialize the client with the given configuration file.
	 * @param config_path configuration file used to initialize the client, can also be a string containing the json configuration
	 * @param available_terminals the set of available terminal implementations.
	 * @param available_requests the set of available requests.
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	ResponsePacket initClient(std::string path, FlyweightTerminalFactory available_terminals, FlyweightRequests available_requests);

	/**
	 * loadAndListReaders - load and list available readers.
	 * The "response" field contains the id and name of each reader.
	 * The "response" field will be formatted this way: ReaderID|ReaderName|...|...
	 * @return a ResponsePacket struct containing either the list of readers or error codes (under 0) and error descriptions.
	 */
	ResponsePacket loadAndListReaders();

	/**
	 * connectClient - connect to the given reader and to the server.
	 * @param reader the reader to use.
	 * @param ip the ip to connect to.
	 * @param port the port to connect to.
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	ResponsePacket connectClient(const char* reader, const char* ip, const char* port);

	/**
	 * disconnectClient - disconnect the cliet from the server and disconnect the terminal.
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	ResponsePacket disconnectClient();

	/**
	 * waitingRequests - wait for requests on the given socket by using the helper function handleRequest.
	 * @return a ResponsePacket struct containing either the request's response or error codes (under 0) and error descriptions.
	 */
	ResponsePacket waitingRequests();

	/**
	 * handleRequest - helper function that performs async actions according to the given request.
	 * The response will be sent back to the given socket.
	 * @param request the request to be performed.
	 * @return a ResponsePacket struct containing either the request's response or error codes (under 0) and error descriptions.
	 */
	ResponsePacket handleRequest(std::string request);

	/**
	 * setStopFlag - set the flag used to stop (or not) waiting for requests.
	 * @param stop_flag the new value of the stop flag.
	 */
	void setConnectedFlag(bool stop_flag);
private:
	ResponsePacket sendResult(std::string result);
};

} /* namespace client */

#endif /* CLIENT_ENGINE_HPP_ */
