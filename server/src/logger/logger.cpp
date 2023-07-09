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
#include "constants/default_values.hpp"
#include "plog/include/plog/Log.h"
#include "plog/include/plog/Appenders/ColorConsoleAppender.h"

#include <windows.h>

namespace logger {

void setup(server::ConfigWrapper* config) {
	std::string log_level = config->getValue("log_level", DEFAULT_LOG_LEVEL);
	int log_max_size = std::stoi(config->getValue("log_max_size", DEFAULT_LOG_MAX_SIZE));
	int log_max_files = std::stoi(config->getValue("log_max_files", DEFAULT_LOG_MAX_FILES));

	std::string log_directory = config->getValue("log_directory", DEFAULT_LOG_DIRECTORY);
	std::string log_filename = config->getValue("log_filename", DEFAULT_LOG_FILENAME);
	std::string log_path = log_directory + "/" + log_filename;

	std::cout << log_path << std::endl;

	CreateDirectory(log_directory.c_str(), NULL);

	if (log_level.compare("debug") == 0) {
		plog::init(plog::debug, log_path.c_str(), log_max_size, log_max_files);
	} else {
		plog::init(plog::info, log_path.c_str(), log_max_size, log_max_files);
	}
}

} /* namespace logger */

