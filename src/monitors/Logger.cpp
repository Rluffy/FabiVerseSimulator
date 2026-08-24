#include "../headers/Logger.h"
#include <iostream>
#include <cstdlib>

Logger::Logger(std::string filename, TimeHandler &timeHandler, bool rewrite)
:timeHandler(timeHandler)
{
 std::filesystem::create_directories(path);
 logFile.open(path / filename, std::ios::out | (rewrite ? std::ios::trunc : std::ios::app));
}

  void Logger::log(LogLevel logLevel, std::string message){
    const char *levelName;

    switch (logLevel)
    {
    case DEBUG:
      levelName = "DEBUG";
      break;
    case INFO:
      levelName = "INFO";
      break;
    case WARNING:
      levelName = "WARNING";
      break;
    case ERROR:
      levelName = "ERROR";
      break;
    default:
      levelName = "UNKNOWN";
      break;
    }

    logFile << "[" << timeHandler.currentDate.toDateString() << "]" << "[" << levelName << "] " << message << std::endl;

  }

  void Logger::closeLogFile(){
    logFile.close();
  }


  


