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

#include "terminal/flyweight_terminal_factory.hpp"
#include "terminal/terminals/terminal.hpp"

#include <map>

namespace client {

void FlyweightTerminalFactory::addFactory(std::string key, ITerminalFactory* factory) {
	factories_.insert(std::make_pair(key, factory));
}

ITerminalFactory* FlyweightTerminalFactory::getFactory(std::string key) {
	if (factories_.count(key) != 1) {
		return NULL;
	}
	return factories_.at(key);
}

} /* namespace client */
