#include "../headers/PersonHandler.h"
#include <algorithm>

PersonHandler::PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler)
    : mapHandler(mapHandler),
      timeHandler(timeHandler)
{

}

void PersonHandler::preparePersons()
{
  Person male(++nextPersonId, {1, 1, 1900}, "Bob", Gender::Male);
  Person female(++nextPersonId, {1, 1, 1900}, "Lara", Gender::Female);

  persons.push_back(male);
  persons.push_back(female);

  mapHandler.insertPersonCoordinate(male, {0,0});
  mapHandler.insertPersonCoordinate(female, {1,10});

}

void PersonHandler::simulatePersons()
{
  for (int p = 0; p < persons.size(); p++)
  {
    Person &person = persons.at(p);
    mapHandler.movePerson(person);

    // check for birth, skip when female pregnant
    if (person.gender == Gender::Female && person.pregnant)
    {
      if (timeHandler.currentDate >= person.babyBirthDate)
      {
        babyBirth(person);
      }

      continue;
    }

    // Check for reproduction partner
    Person* repPartner = getReproductionPartner(person);
    // Only reproduce if found
    if (repPartner != nullptr)
    {
      reproduce(person, *repPartner);
    }
  }
}

void PersonHandler::babyBirth(Person &p)
{
  Coordinate *birthLocationP = mapHandler.getNextFreePostion();
  p.pregnant = false;

  // Baby only born when free space found
  if (birthLocationP != nullptr)
  {
    Coordinate birthLocation = *birthLocationP;
    delete birthLocationP;
    // 1/2 for male or female
    Gender babyGender = rand() % 2 == 0 ? Gender::Male : Gender::Female;
    Person baby(++nextPersonId, p.babyBirthDate, to_string(nextPersonId), babyGender);
    persons.push_back(baby);
    mapHandler.insertPersonCoordinate(baby, birthLocation);
  }
  else
  {
    cout << "Full";
  }
}

Person *PersonHandler::getReproductionPartner(const Person &person)
{
  vector<int> neighbIds = mapHandler.getPersonNeighbourIds(person);
  for (int neighbId : neighbIds)
  {
    auto neighIt = std::find_if(persons.begin(), persons.end(), [&](const Person &p)
                                { return p.id == neighbId; });
    if (neighIt != persons.end())
    {
      Person &neighbour = *neighIt;
      if (Person::reproductionPossible(person, neighbour))
      {
        return &neighbour;
      }
    }
  }
  return nullptr;
}

void PersonHandler::reproduce(Person &p1, Person &p2)
{
  if (p1.gender == Gender::Female)
  {
    setPersonPregnant(p1);
  }
  else if (p2.gender == Gender::Female)
  {
    setPersonPregnant(p2);
  }
}

void PersonHandler::setPersonPregnant(Person &p1)
{
  p1.pregnant = true;
  p1.pregnancyDate = timeHandler.currentDate;
  p1.babyBirthDate = timeHandler.currentDate;
  p1.babyBirthDate.addMonths(timeHandler.pregnancyTimeMonths);
}
