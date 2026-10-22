#include "../headers/Logger.h"
#include <ctime>

Logger::Logger(LogConfig conf, TimeHandler &timeHandler)
    : conf(conf),
      timeHandler(timeHandler)
{
  logFile.open(conf.path + "/" + conf.file, std::ios::out | (conf.rewrite ? std::ios::trunc : std::ios::app));
}

bool Logger::isEnabled(LogLevel logLevel) const
{
  return logLevel >= conf.minLogLevel;
}

void Logger::log(LogLevel logLevel, const std::string &message, bool printCurrentTime)
{
  if (!isEnabled(logLevel))
  {
    return;
  }

  const char *levelName;

  switch (logLevel)
  {
  case DEBUG:
    levelName = "[DEBUG]  ";
    break;
  case INFO:
    levelName = "[INFO]   ";
    break;
  case WARNING:
    levelName = "[WARNING]";
    break;
  case ERROR:
    levelName = "[ERROR]  ";
    break;
  default:
    levelName = "[UNKNOWN]";
    break;
  }

  logFile << "[" << timeHandler.currentDate.toDateString() << "]" << levelName << message;

  if (printCurrentTime)
  {
    std::time_t timestamp = std::time(nullptr);
    logFile << " Current Time " << std::ctime(&timestamp);
  }

  // '\n' instead of std::endl: endl flushes the file on every line
  logFile << '\n';
}

void Logger::closeLogFile()
{
  logFile.close();
}
