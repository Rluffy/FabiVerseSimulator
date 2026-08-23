#include "../headers/PersonHandler.h"
#include <algorithm>

PersonHandler::PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler)
    : mapHandler(mapHandler),
      timeHandler(timeHandler)
{
}

void PersonHandler::preparePersons()
{
  cout << "Prepare Persons" << endl;
  Person male(++nextPersonId, {1, 1, 1900}, "Bob", Gender::Male);
  Person female(++nextPersonId, {1, 1, 1900}, "Lara", Gender::Female);
  mapHandler.insertPersonCoordinate(male, {0, 0});
  mapHandler.insertPersonCoordinate(female, {1, 10});

  persons.push_back(male);
  persons.push_back(female);
}

void PersonHandler::simulatePersons()
{
  // cout << "Simulate Persons" << endl;
  // 24 hours
  for (int i = 0; i < 24; i++)
  {
    for (int p = 0; p < persons.size(); p++)
    {
      Person &person = persons.at(p);
      mapHandler.movePerson(person);

      // Check for reproduction partner
      Person *repPartner = getReproductionPartner(person);
      // Only reproduce if found
      if (repPartner != nullptr)
      {
        reproduce(person, *repPartner);
      }
    }
  }
  processBirths();
}

void PersonHandler::babyBirth(Person &baby)
{

  // cout << "Baby born with Id: " << baby.id 
  // << " Mother Id: " << baby.motherId << " Father Id: " << baby.fatherId  << " " << timeHandler.currentDate.toDateString() << endl;
  // find mother and set unpregnant
  auto motherIt = std::find_if(persons.begin(), persons.end(), [&](const Person &p)
                               { return p.id == baby.motherId; });
  if (motherIt != persons.end())
  {
    Person &mother = *motherIt;
    mother.pregnant = false;
  }

  // find free coordinates
  Coordinate *birthLocationP = mapHandler.getNextFreePostion();

  // Baby only born when free space found
  if (birthLocationP != nullptr)
  {
    Coordinate birthLocation = *birthLocationP;
    delete birthLocationP;

    mapHandler.insertPersonCoordinate(baby, birthLocation);
    persons.push_back(baby);
  }
  else
  {
    cout << "Full" << endl;
  }
}

Person *PersonHandler::getReproductionPartner(const Person &person)
{
  // cout << "Get Reproduction Partner for Person Id: " << person.id << endl;
  vector<int> neighbIds = mapHandler.getPersonNeighbourIds(person);
  for (int neighbId : neighbIds)
  {
    auto neighIt = std::find_if(persons.begin(), persons.end(), [&](const Person &p)
                                { return p.id == neighbId; });
    if (neighIt != persons.end())
    {
      Person &neighbour = *neighIt;
      if (reproductionPossible(person, neighbour))
      {
        // cout << "Found Reproduction Partner for Person Id: " << person.id << " Partner Id: " << neighbour.id << endl;
        return &neighbour;
      }
    }
  }
  // cout << "No Reproduction Partner found for Person Id: " << person.id << endl;
  return nullptr;
}

void PersonHandler::reproduce(Person &p1, Person &p2)
{
  // cout << "Reproduce Person Id: " << p1.id << " with Person Id: " << p2.id << endl;
  Date birthDate;
  int fatherId;
  int motherId;

  if (p1.gender == Gender::Female)
  {
    makePersonPregnant(p1);
    birthDate = p1.babyBirthDate;
    motherId = p1.id;
    fatherId = p2.id;
  }
  else if (p2.gender == Gender::Female)
  {
    makePersonPregnant(p2);
    birthDate = p2.babyBirthDate;
    motherId = p2.id;
    fatherId = p1.id;
  }

  // create baby
  // 1/2 for male or female
  Gender babyGender = rand() % 2 == 0 ? Gender::Male : Gender::Female;
  Person baby(++nextPersonId, birthDate, to_string(nextPersonId), babyGender);
  baby.fatherId = fatherId;
  baby.motherId = motherId;
  babies.push_back(baby);
}

void PersonHandler::processBirths()
{
  // cout << "Babie Size " << babies.size();
  vector<Person> unbornBabies;
  for (  Person &babie : babies)
  {
    bool birthTime = timeHandler.currentDate >= babie.birthdate;
    if (birthTime)
    {
      babyBirth(babie);
    }
    else
    {
      unbornBabies.push_back(babie);
    }
  }
  babies = unbornBabies;
}
void PersonHandler::makePersonPregnant(Person &p1)
{
  // cout << "Make Person Id: " << p1.id << " Pregnant" << endl;
  p1.pregnant = true;
  p1.pregnancyDate = timeHandler.currentDate;
  p1.babyBirthDate = timeHandler.currentDate;
  p1.babyBirthDate.addMonths(timeHandler.pregnancyTimeMonths);
}

bool PersonHandler::reproductionPossible(const Person &p1, const Person &p2)
{
  // Different sex and female not pregnant
  return p1.gender != p2.gender && !p1.pregnant && !p2.pregnant;
}
