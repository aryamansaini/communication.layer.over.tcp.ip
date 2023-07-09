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

#include "constants/request_code.hpp"
#include "constants/response_packet.hpp"
#include "server/server_api.hpp"
#include "server/server_engine.hpp"
#include "nlohmann/json.hpp"

#include "plog/include/plog/Log.h"
#include "plog/include/plog/Appenders/ColorConsoleAppender.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

namespace server {

ResponsePacket ServerAPI::initServer(std::string path) {
	return engine_->initServer(path);
}

ResponsePacket ServerAPI::getVersion() {
	ResponsePacket response_packet = { .response = DLL_VERSION, .err_server_code = ERR_INVALID_STATE, .err_server_description = "Server invalid state" };

	return response_packet;
}

ResponsePacket ServerAPI::startServer(const char* ip, const char* port) {
	return engine_->startListening(ip, port);
}

ResponsePacket ServerAPI::listClients() {
	return engine_->listClients();
}

ResponsePacket ServerAPI::sendCommand(int id_client, std::string command, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_COMMAND, true, timeout, command);
}

ResponsePacket ServerAPI::sendTypeA(int id_client, std::string command, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_COMMAND_A, true, timeout, command);
}

ResponsePacket ServerAPI::sendTypeB(int id_client, std::string command, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_COMMAND_B, true, timeout, command);
}

ResponsePacket ServerAPI::sendTypeF(int id_client, std::string command, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_COMMAND_F, true, timeout, command);
}

ResponsePacket ServerAPI::restartTarget(int id_client, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_RESTART, timeout);
}

ResponsePacket ServerAPI::echoClient(int id_client, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_ECHO, timeout);
}

ResponsePacket ServerAPI::diagClient(int id_client, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_DIAG, timeout);
}

ResponsePacket ServerAPI::stopClient(int id_client, DWORD timeout) {
	ResponsePacket response = engine_->handleRequest(id_client, REQ_DISCONNECT, false, timeout);
	if (response.err_server_code == ERR_NETWORK) {
		ResponsePacket okResponse;
		return okResponse;
	}
	return response;
}

ResponsePacket ServerAPI::coldReset(int id_client, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_COLD_RESET, true, timeout);
}

ResponsePacket ServerAPI::warmReset(int id_client, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_WARM_RESET, true, timeout);
}

ResponsePacket ServerAPI::powerOFFField(int id_client, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_POWER_OFF_FIELD, true, timeout);
}

ResponsePacket ServerAPI::powerONField(int id_client, DWORD timeout) {
	return engine_->handleRequest(id_client, REQ_POWER_ON_FIELD, true, timeout);
}

ResponsePacket ServerAPI::stopServer() {
	return engine_->stopAllClients();
}

}  // namespace server

