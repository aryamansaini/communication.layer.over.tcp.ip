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

#include "client/client_api.hpp"
#include "client/requests/cold_reset.hpp"
#include "client/requests/command.hpp"
#include "client/requests/diag.hpp"
#include "client/requests/disconnect.hpp"
#include "client/requests/echo.hpp"
#include "client/requests/power_off_field.hpp"
#include "client/requests/power_on_field.hpp"
#include "client/requests/request.hpp"
#include "client/requests/request.hpp"
#include "client/requests/restart_target.hpp"
#include "client/requests/send_typeA.hpp"
#include "client/requests/send_typeB.hpp"
#include "client/requests/send_typeF.hpp"
#include "client/requests/warm_reset.hpp"
#include "constants/request_code.hpp"
#include "terminal/factories/example_factory_pcsc_contact.hpp"
#include "terminal/factories/example_factory_pcsc_contactless.hpp"
#include "terminal/flyweight_terminal_factory.hpp"
#include "terminal/terminals/example_pcsc_contact.hpp"

using namespace client;

int __cdecl main(void) {
	// config available terminal factories
	FlyweightTerminalFactory available_terminals;
	available_terminals.addFactory("EXAMPLE_PCSC_CONTACT", new ExamplePCSCContactFactory());
	available_terminals.addFactory("EXAMPLE_PCSC_CONTACTLESS", new ExamplePCSCContactlessFactory());

	// config all requests the client can handle
	FlyweightRequests available_requests;
	available_requests.addRequest(REQ_COMMAND, new Command());
	available_requests.addRequest(REQ_COMMAND_A, new SendTypeA());
	available_requests.addRequest(REQ_COMMAND_B, new SendTypeB());
	available_requests.addRequest(REQ_COMMAND_F, new SendTypeF());
	available_requests.addRequest(REQ_DIAG, new Diag());
	available_requests.addRequest(REQ_DISCONNECT, new Disconnect());
	available_requests.addRequest(REQ_ECHO, new Echo());
	available_requests.addRequest(REQ_RESTART, new RestartTarget());
	available_requests.addRequest(REQ_COLD_RESET, new ColdReset());
	available_requests.addRequest(REQ_WARM_RESET, new WarmReset());
	available_requests.addRequest(REQ_POWER_OFF_FIELD, new PowerOffField());
	available_requests.addRequest(REQ_POWER_ON_FIELD, new PowerOnField());

	ClientAPI* client = new ClientAPI(0, 0, 0);
	client->initClient("./config/init.json", available_terminals, available_requests);
//	client->connectClient("SCM Microsystems Inc. SCL010 Contactless Reader 0", "127.0.0.1", "62111");
//	client->connectClient("NXP NXP's Proximity based PCSC Reader 0", "127.0.0.1", "62111");
	client->connectClient("Alcor Micro USB Smart Card Reader 0", "127.0.0.1", "62111");
	Sleep(100000);
	return 0;
}
