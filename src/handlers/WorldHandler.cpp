#include "../headers/WorldHandler.h"

#include <thread>
#include <chrono>

using namespace std;

WorldHandler::WorldHandler(World world)
    : world(world),
      th(world.startDate, world.pregnancyTimeMonths),
      mh(world.length, world.width),
      logger("fabiVerseLog", th, true),
      ph(mh, th, logger)

{
}

void WorldHandler::startSimulation()
{
  prepareSimulation();

  logger.log(LogLevel::INFO, "Start simulating");
  for (int i = 0; i < 4000; i++)
  {
    simulateDay();
    th.nextDay(); 
  }

  logger.log(LogLevel::INFO, "Person count: " + to_string(ph.persons.size()));
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
