#pragma once

#include <fstream>
#include <string>
#include "TimeHandler.h"
#include "../configs/LogConfig.h"

enum LogLevel
{
  DEBUG,
  INFO,
  WARNING,
  ERROR
};

class Logger
{
public:
  Logger(LogConfig conf, TimeHandler &timeHandler);
  // Lets callers skip building expensive messages that would be dropped anyway
  bool isEnabled(LogLevel logLevel) const;
  void log(LogLevel logLevel, const std::string &message, bool printCurrentTime = false);
  void closeLogFile();

private:
  std::ofstream logFile;
  LogConfig conf;
  TimeHandler &timeHandler;
};
