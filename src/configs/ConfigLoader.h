#pragma once

#include <string>
#include "Config.h"

// Reads all settings from the XML file. Throws std::runtime_error when the file cannot be read.
Config loadConfig(const std::string &path);
