#include "../headers/World.h"

#include <thread>
#include <chrono>

using namespace std;

World::World(int id, std::string name, Date startDate, int length, int width)
    : id(id),
      name(name),
      startDate(startDate),
      length(length),
      width(width)
{
  this->currentDate = startDate;
  this->temperature = 20.0;
}

void World::startSimulation()
{
  prepareSimulation();
  while (true)
  {

    // 24 hours
    for (int i = 0; i < 24; i++)
    {
      for (Person &person : persons)
      {
        Coordinate cord = person.move(length, width);

        // Only move person when cordinates are free
        if (objectCoordinates.find(cord) == objectCoordinates.end())
        {
          // Update coordinate info
          objectCoordinates.erase(person.coordinate);
          person.coordinate = cord;
          objectCoordinates.insert({cord, person});

          // check for birth, skip when pregnant
          if (person.gender == Gender::Female && person.pregnant){
            if ( person.babyBirthDate  >= currentDate){
              babyBirth(person);

            }

            continue;
          }

          // check for reproduction
          Person* repPartner = getReproductionPartner(person);
          // Only reproduce if found
          if (repPartner != nullptr)
          {
            reproduce(person, *repPartner);
          }
        }
      }
    }

    this_thread::sleep_for(chrono::microseconds(1));
    currentDate.nextDay();
  }
}

Person* World::getReproductionPartner(const Person &person)
{
  int xP = person.coordinate.x;
  int yP = person.coordinate.y;
  int xNextPos, yNextPos;
  for (int x = -1; x < 2; x++)
  {
    xNextPos = xP + x;

    for (int y = -1; y < 2; y++)
    {
      // Skip same position
      if (x == 0 && y == 0)
      {
        continue;
      }
      yNextPos = yP + y;

      auto it = objectCoordinates.find({xNextPos, yNextPos});
      // person found
      if (it != objectCoordinates.end())
      {
        Person &neighbour = it->second;
        if (reproductionPossible(person, neighbour))
        {
          return &neighbour;
        }
      }
    }
  }
  return nullptr;
}

void World::reproduce(Person &p1, Person &p2)
{
  if (p1.gender == Gender::Female)
  {
    p1.pregnant = true;
    p1.pregnancyDate = currentDate;
    p1.babyBirthDate = currentDate;
    p1.babyBirthDate.addMonths(pregnancyTimeMonths);
  }
  else if (p2.gender == Gender::Female)
  {
    p2.pregnant = true;
    p2.pregnancyDate = currentDate;
    p2.babyBirthDate.addMonths(pregnancyTimeMonths);
  }
}

bool World::reproductionPossible(const Person &p1, const Person &p2)
{
  // Different sex
  return p1.gender != p2.gender;
}

void World::babyBirth(Person &p1){
  p1.babyBirthDate = {0,0,0};
  p1.pregnant = false;

  cout << "Baby was made on" << p1.pregnancyDate.toDateString() << " and is born on "  << p1.babyBirthDate.toDateString();
}

void World::prepareSimulation()
{
  Person male(0, {1, 1, 1900}, "Bob", Gender::Male, {0, 0});
  Person female(0, {1, 1, 1900}, "Lara", Gender::Female, {3, 3});

  persons.push_back(male);
  persons.push_back(female);
}
