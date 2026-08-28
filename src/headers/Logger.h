#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include "TimeHandler.h"
#include "../configs/LogConfig.h"

enum LogLevel{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};


class Logger {
public:
  Logger(LogConfig conf, TimeHandler &timeHandler);
  void log(LogLevel logLevel, std::string message);
  void closeLogFile();
 
  private:
  std::ofstream logFile;
  LogConfig conf;
  //std::filesystem::path path = std::filesystem::path(std::getenv("HOME")) / "logs";
  TimeHandler &timeHandler;
};

        