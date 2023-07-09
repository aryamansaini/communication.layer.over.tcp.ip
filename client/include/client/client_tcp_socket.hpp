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

#ifndef INCLUDE_CLIENT_CLIENT_TCP_SOCKET_HPP_
#define INCLUDE_CLIENT_CLIENT_TCP_SOCKET_HPP_

#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

namespace client {

class ClientTCPSocket {
private:
	SOCKET client_socket_;
	WSADATA wsaData_;
	const char* ip_;
	const char* port_;
	struct addrinfo* result_;
	struct addrinfo hints_;
private:
	bool sendData(const char* data, int size);
public:
	ClientTCPSocket() = default;
	~ClientTCPSocket() = default;

	/**
	 * initClient - initialises Winsock and socket parameters
	 * @param ip the ip to connect to.
	 * @param port the port to connect to.
	 * @return a boolean indicating whether an error occurred.
	 */
	bool initClient(const char* ip, const char* port);

	/**
	 * connectClient - connect the client to the server.
	 * @return a boolean indicating whether an error occurred.
	 */
	bool connectClient();

	/**
	 * sendPacket - send packet on the socket.
	 * @param packet the packet to be sent.
	 * @return a boolean indicating whether an error occurred.
	 */
	bool sendPacket(const char* packet);

	/**
	 * receivePacket - receivePacket packet on the socket.
	 * @param packet the packet to be received.
	 * @return a boolean indicating whether an error occurred.
	 */
	bool receivePacket(char* packet);

	/**
	 * closeClient - cleanup and close socket.
	 */
	void closeClient();
};

} /* namespace client */

#endif /* INCLUDE_CLIENT_CLIENT_TCP_SOCKET_HPP_ */
