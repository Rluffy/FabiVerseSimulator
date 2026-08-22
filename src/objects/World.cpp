#include "../headers/World.h"

using namespace std;

World::World(int id, string name, Date startDate, int length, int width, double temperature)
    : id(id),
      name(name),
      startDate(startDate),
      length(length),
      width(width),
      temperature(temperature)
{

}