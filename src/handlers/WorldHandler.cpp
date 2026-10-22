#include "../headers/WorldHandler.h"

#include <iostream>

using namespace std;

WorldHandler::WorldHandler(Config conf)
    : th(conf.timeConfig),
      mh(conf.mapConfig, random),
      logger(conf.logConfig, th),
      fh(conf.foodConfig, mh),
      ph(mh, th, logger, random, fh, conf.personConfig),
      conf(conf.worldConfig)
{
}

void WorldHandler::startSimulation()
{
  prepareSimulation();

  logger.log(LogLevel::INFO, "Start simulating", true);
  cout << "Start simulating" << endl;
  for (int i = 0; i < conf.simDurationDays; i++)
  {
    simulateDay();
    th.nextDay();

    if (ph.getPersonsSize() <= 0)
    {
      logger.log(LogLevel::WARNING, "[STOPPED] Cancel simulating, all persons died", true);
      break;
    }
  }

  ph.logPersonStatistics();

  cout << "Finished" << endl;
  logger.closeLogFile();
}

void WorldHandler::simulateDay()
{
  ph.simulateDay();
  fh.spawnFood();
}

void WorldHandler::prepareSimulation()
{
  ph.preparePersons();
}
