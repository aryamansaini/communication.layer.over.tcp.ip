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

#include "config/config_wrapper.hpp"
#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace client {

void ConfigWrapper::init(std::string path) {
	std::ifstream i(path);
	i >> config_;
}

void ConfigWrapper::initFromJson(std::string jsonConfig) {
    config_ = nlohmann::json::parse(jsonConfig);
}


std::string ConfigWrapper::getValue(std::string key) {
	return config_[key];
}

std::string ConfigWrapper::getValue(std::string key, std::string default_value) {
	return config_[key].is_null() ? default_value : config_[key].get<std::string>();
}

} /* namespace client */
