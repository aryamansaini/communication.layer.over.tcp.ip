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

#ifndef TERMINAL_LAYER_H_
#define TERMINAL_LAYER_H_

#include "constants/response_packet.hpp"

#include <windows.h>

namespace client {

class ITerminalLayer {
public:
	ITerminalLayer() = default;
	virtual ~ITerminalLayer() {};

	/**
	 * init - initialize the terminal.
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket init() = 0;

	/**
	 * loadAndListReaders - load and list available readers.
	 * The "response" field contains the id and name of each reader.
	 * The "response" field will be formatted this way: ReaderID|ReaderName|...|...
	 * @return a ResponsePacket struct containing either the list of readers or error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket loadAndListReaders() = 0;

	/**
	 * connect - connect to the given reader.
	 * @param reader
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket connect(const char* reader) = 0;

	/**
	 * sendCommand - send an APDU command to the terminal.
	 * @param command the command to be sent to the smartcard.
	 * @param command_length the length of the given command.
	 * @return a ResponsePacket struct containing either the smartcard's response or error codes and error descriptions in case of error.
	 */
	virtual ResponsePacket sendCommand(unsigned char command[],  unsigned long int command_length) = 0;

	/**
	 * sendTypeA - send an APDU command over RF Type A
	 * @param command the APDU command to be sent to the smartcard.
	 * @param command_length the length of the given APDU command.
	 * @return a ResponsePacket struct containing either the smartcard's response or error codes and error descriptions in case of error.
	 */
	virtual ResponsePacket sendTypeA(unsigned char command[],  unsigned long int command_length) = 0;

	/**
	 * sendTypeB - send an APDU command over RF Type B
	 * @param command the APDU command to be sent to the smartcard.
	 * @param command_length the length of the given APDU command.
	 * @return a ResponsePacket struct containing either the smartcard's response or error codes and error descriptions in case of error.
	 */
	virtual ResponsePacket sendTypeB(unsigned char command[],  unsigned long int command_length) = 0;

	/**
	 * sendTypeF - send an APDU command over RF Type F
	 * @param command the APDU command to be sent to the smartcard.
	 * @param command_length the length of the given APDU command.
	 * @return a ResponsePacket struct containing either the smartcard's response or error codes and error descriptions in case of error.
	 */
	virtual ResponsePacket sendTypeF(unsigned char command[],  unsigned long int command_length) = 0;

	/**
	 * diag - diagnose the used terminal.
	 * The diagnostic will be in the field "response" of the ResponsePacket structure.
	 * The diagnostic will be formatted this way: TheReaderName|TheReaderStatus|TheReaderProtocolUsed
	 * @return a ResponsePacket struct containing either the data or error codes and error descriptions.
	 */
	virtual ResponsePacket diag() = 0;

	/**
	 * disconnect - disconnect the terminal.
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket disconnect() = 0;

	/**
	 * isAlive - send a valid packet to the terminal to check whether it is responding.
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket isAlive() = 0;

	/**
	 * restart - restart the TOE.
	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket restart() = 0;

	/**
	 * coldReset - perform power off power on and return atr in the response structure.
	 * @return a ResponsePacket struct containing either the atr or the error codes (under 0) and error descriptions
	 */
	virtual ResponsePacket coldReset() = 0;

	/**
	 * coldReset - perform the reset without power switch and return atr in the response structure.
	 * @return a ResponsePacket struct containing either the atr or the error codes (under 0) and error descriptions
	 */
	virtual ResponsePacket warmReset() = 0;

	/**
	 * powerOFFField - set the field OFF
 	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket powerOFFField() = 0;

	/**
	 * powerONField - set the field ON
 	 * @return a ResponsePacket struct containing possible error codes (under 0) and error descriptions.
	 */
	virtual ResponsePacket powerONField() = 0;
};

} /* namespace client */

#endif /* TERMINAL_LAYER_H_ */
