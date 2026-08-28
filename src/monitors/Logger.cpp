#include "../headers/Logger.h"
#include <iostream>
#include <cstdlib>

Logger::Logger(LogConfig conf, TimeHandler &timeHandler)
:conf(conf),
timeHandler(timeHandler)
{
 logFile.open(conf.path + "/" + conf.file, std::ios::out | (conf.rewrite ? std::ios::trunc : std::ios::app));
}

  void Logger::log(LogLevel logLevel, std::string message){
    const char *levelName;

    if (logLevel < conf.minLogLevel){
      return;
    }

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


  


