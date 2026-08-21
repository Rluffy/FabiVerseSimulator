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

  for (int i = 0; i < 100000; i++)
  {
    simulateDay();
  }

  cout << " Personenanzahl: " << persons.size();
}

void World::simulateDay()
{
  // 24 hours
  for (int i = 0; i < 24; i++)
  {
    for (int p = 0; p < persons.size(); p++)
    {

      Person &person = persons.at(p);
      Coordinate cord = person.move(length, width);

      // Only move person when cordinates are free
      if (objectCoordinates.find(cord) != objectCoordinates.end())
      {
        continue;
      }
      // Update coordinate info
      objectCoordinates.erase(person.coordinate);
      person.coordinate = cord;
      objectCoordinates.insert({cord, &person});

      // check for birth, skip when female pregnant
      if (person.gender == Gender::Female && person.pregnant)
      {
        if (person.babyBirthDate >= currentDate)
        {
          babyBirth(person);
        }

        continue;
      }

      // Check for reproduction partner
      Person *repPartner = getReproductionPartner(person);
      // Only reproduce if found
      if (repPartner != nullptr)
      {
        reproduce(person, *repPartner);
      }
    }
  }

  this_thread::sleep_for(chrono::microseconds(1));
  currentDate.nextDay();
}
Person *World::getReproductionPartner(const Person &person)
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
        // cout << "Partner found";
        Person &neighbour = *(it->second);
        if (reproductionPossible(person, neighbour))
        {
          // cout << "Neighbour";
          return &neighbour;
        }
      }
    }
  }
  return nullptr;
}

Coordinate *World::getNextFreePostion()
{
  for (int x = 0; x < length; x++)
  {
    for (int y = 0; y < width; y++)
    {
      Coordinate cord = {x, y};
      auto it = objectCoordinates.find(cord);
      // Free postion found
      if (it == objectCoordinates.end())
      {
        return new Coordinate{x, y};
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
    p2.babyBirthDate = currentDate;
    p2.babyBirthDate.addMonths(pregnancyTimeMonths);
  }
}

bool World::reproductionPossible(const Person &p1, const Person &p2)
{
  // Different sex and female not pregnant
  return p1.gender != p2.gender && !p1.pregnant && !p2.pregnant;
}

void World::babyBirth(Person &p1)
{
  Coordinate *birthLocationP = getNextFreePostion();
  p1.pregnant = false;

  // Baby only born when free space found
  if (birthLocationP != nullptr)
  {
    Coordinate birthLocation = *birthLocationP;
    delete birthLocationP;
    // 1/2 for male or female
    Gender babyGender = rand() % 2 == 0 ? Gender::Male : Gender::Female;
    Person baby(++nextPersonId, p1.babyBirthDate, to_string(nextPersonId), babyGender, birthLocation);
    persons.push_back(baby);
    objectCoordinates.insert({birthLocation, &baby});
  }
  else
  {
    cout << "Full";
  }
}

void World::prepareSimulation()
{
  Person male(++nextPersonId, {1, 1, 1900}, "Bob", Gender::Male, {0, 0});
  Person female(++nextPersonId, {1, 1, 1900}, "Lara", Gender::Female, {3, 3});

  persons.push_back(male);
  persons.push_back(female);

  objectCoordinates.insert({{0, 0}, &male});
  objectCoordinates.insert({{3, 3}, &female});
}
