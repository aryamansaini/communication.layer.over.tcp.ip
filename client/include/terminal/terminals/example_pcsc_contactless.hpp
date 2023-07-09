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

#ifndef TERMINAL_TERMINALS_TERMINAL_PCSC_CONTACTLESS_H_
#define TERMINAL_TERMINALS_TERMINAL_PCSC_CONTACTLESS_H_

#define DEFAULT_BUFLEN 1024 * 64

#include "terminal/terminals/terminal.hpp"
#include "constants/response_packet.hpp"

#include <map>
#include <string>
#include <winscard.h>

namespace client {

class ExampleTerminalPCSCContactless: public ITerminalLayer {
private:
	std::string current_reader_;
	SCARDCONTEXT hContext_;
	LPTSTR mszReaders_;
	SCARDHANDLE hCard_;
	DWORD dwReaders_, dwActiveProtocol_, dwRecvLength_;
	SCARD_IO_REQUEST pioSendPci_;
	BYTE pbRecvBuffer_[DEFAULT_BUFLEN];
public:
	ExampleTerminalPCSCContactless() = default;
	~ExampleTerminalPCSCContactless();
	ResponsePacket init() override;
	ResponsePacket loadAndListReaders() override;
	ResponsePacket connect(const char* reader) override;
	ResponsePacket sendCommand(unsigned char command[], unsigned long int command_length) override;
	ResponsePacket sendTypeA(unsigned char command[], unsigned long int command_length) override;
	ResponsePacket sendTypeB(unsigned char command[], unsigned long int command_length) override;
	ResponsePacket sendTypeF(unsigned char command[], unsigned long int command_length) override;
	ResponsePacket diag() override;
	ResponsePacket disconnect() override;
	ResponsePacket isAlive() override;
	ResponsePacket restart() override;
	ResponsePacket coldReset() override;
	ResponsePacket warmReset() override;
	ResponsePacket powerOFFField() override;
	ResponsePacket powerONField() override;
private:
	ResponsePacket handleErrorResponse(std::string context_message, LONG error);
	ResponsePacket retrieveAtr(BYTE* bAttr, DWORD* cByte);
	std::string errorToString(LONG error);
	LONG handleRetry();
};

} /* namespace client */

#endif /* TERMINAL_TERMINALS_TERMINAL_PCSC_CONTACTLESS_H_ */
