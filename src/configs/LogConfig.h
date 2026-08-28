#pragma once

#include <string>
struct LogConfig
{
    std::string file;
    std::string path;
    int minLogLevel;
    bool rewrite;
};

