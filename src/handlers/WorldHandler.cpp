#include "../headers/WorldHandler.h"


#include <thread>
#include <chrono>

using namespace std;

WorldHandler::WorldHandler(Config conf)
    : th(conf.timeConfig),
      mh(conf.mapConfig),
      logger(conf.logConfig, th),
      ph(mh, th, logger, conf.personConfig),
      conf(conf.worldConfig)
{
}

void WorldHandler::startSimulation()
{
  prepareSimulation();

  logger.log(LogLevel::INFO, "Start simulating");
  cout << "Start simulating" << endl;
  for (int i = 0; i < conf.simDurationDays; i++)
  {
    simulateDay();
    th.nextDay(); 
  }

  logger.log(LogLevel::INFO, "Person count: " + to_string(ph.persons.size()));
  logger.log(LogLevel::INFO, "Overall killed count: " + to_string(ph.overallPersonsKilled));
  logger.log(LogLevel::INFO, "Overall babys born: " + to_string(ph.overallBabyCount));
  logger.log(LogLevel::INFO, "Overall sex count: " + to_string(ph.overallSexCount));
  logger.log(LogLevel::INFO, "Overall persons lived count: " + to_string(ph.overallBabyCount + ph.conf.startPersonCount));
  logger.log(LogLevel::INFO, "Overall persons died count: " + to_string(ph.overallDeathCount));


  cout << "Finished" << endl;
  logger.closeLogFile();

}

void WorldHandler::simulateDay()
{
    ph.simulatePersons();
}

void WorldHandler::prepareSimulation()
{
  srand(time(0));
  ph.preparePersons();
}
