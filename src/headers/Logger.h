#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include "TimeHandler.h"

enum LogLevel{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};


class Logger {
public:
  Logger(std::string filename, TimeHandler &timeHandler, bool rewrite = false);
  void log(LogLevel logLevel, std::string message);
  void closeLogFile();
 
  private:
  std::ofstream logFile;
  std::filesystem::path path = std::filesystem::path(std::getenv("HOME")) / "logs";
  TimeHandler &timeHandler;
};

        