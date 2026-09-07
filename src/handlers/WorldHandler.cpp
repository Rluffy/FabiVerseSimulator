#include "../headers/WorldHandler.h"

#include <thread>
#include <chrono>

using namespace std;

WorldHandler::WorldHandler(Config conf)
    : th(conf.timeConfig),
      mh(conf.mapConfig),
      logger(conf.logConfig, th),
      ph(mh, th, logger, conf.personConfig),
      fh(conf.foodConfig, mh),
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
  ph.simulatePersons();
  fh.spawnFood();
}

void WorldHandler::prepareSimulation()
{
  srand(time(0));
  ph.preparePersons();
}
