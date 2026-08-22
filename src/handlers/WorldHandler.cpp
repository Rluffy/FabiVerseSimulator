#include "../headers/WorldHandler.h"

#include <thread>
#include <chrono>

using namespace std;

WorldHandler::WorldHandler(World world)
    : world(world),
      th(world.currentDate, world.pregnancyTimeMonths),
      mh(world.length, world.width),
      ph(mh, th)
{
}

void WorldHandler::startSimulation()
{
  prepareSimulation();

  for (int i = 0; i < 1000; i++)
  {
    simulateDay();
    th.nextDay(); 
  }

  cout << " Personenanzahl: " << ph.persons.size();
}

void WorldHandler::simulateDay()
{
  // 24 hours
  for (int i = 0; i < 24; i++)
  {
    ph.simulatePersons();
  }
}

void WorldHandler::prepareSimulation()
{
  srand(time(0));
  ph.preparePersons();
}
