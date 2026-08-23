#include "../headers/WorldHandler.h"

#include <thread>
#include <chrono>

using namespace std;

WorldHandler::WorldHandler(World world)
    : world(world),
      th(world.startDate, world.pregnancyTimeMonths),
      mh(world.length, world.width),
      ph(mh, th)
{
}

void WorldHandler::startSimulation()
{
  prepareSimulation();

  for (int i = 0; i < 500; i++)
  {
    simulateDay();
    th.nextDay(); 
  }

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
