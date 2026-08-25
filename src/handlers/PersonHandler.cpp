#include "../headers/PersonHandler.h"
#include <algorithm>

PersonHandler::PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler, Logger &logger)
    : mapHandler(mapHandler),
      timeHandler(timeHandler),
      logger(logger)
{
}

void PersonHandler::preparePersons()
{
  logger.log(LogLevel::INFO, "Prepare Persons");
  Person male(++nextPersonId, {1, 1, 1900}, "Bob", Gender::Male);
  Person female(++nextPersonId, {1, 1, 1900}, "Lara", Gender::Female);
  mapHandler.insertPersonCoordinate(male, {0, 0});
  mapHandler.insertPersonCoordinate(female, {1, 10});

  persons.push_back(male);
  persons.push_back(female);
}

void PersonHandler::simulatePersons()
{
  for (int i = 0; i < 24; i++)
  {
    for (int p = 0; p < persons.size(); p++)
    {
      Person &person = persons.at(p);
      mapHandler.movePerson(person);

      // Check for reproduction partner
      Person *repPartner = getReproductionPartner(person);
      // Only reproduce if found
      if (repPartner)
      {
        reproduce(person, *repPartner);
      }
    }
  }
  processBirths();
}

void PersonHandler::babyBirth(Person &baby)
{
  auto motherIt = std::find_if(persons.begin(), persons.end(), [&](const Person &p)
                               { return p.id == baby.motherId; });

  Person *motherP = getPersonById(baby.motherId);
  if (motherP)
  {
    Person &mother = *motherP;
    mother.pregnant = false;
  }

  // find free coordinates
  Coordinate *birthLocationP = mapHandler.getNextFreePostion();

  // Baby only born when free space found
  if (birthLocationP)
  {
    Coordinate birthLocation = *birthLocationP;
    delete birthLocationP;

    mapHandler.insertPersonCoordinate(baby, birthLocation);
    persons.push_back(baby);

    logger.log(LogLevel::DEBUG, "Baby born, id: " + to_string(baby.id) + " birthdate: " + baby.birthdate.toDateString() + " mother id: " + to_string(baby.motherId) + " father id: " + to_string(baby.fatherId));
  }
  else
  {
    logger.log(LogLevel::WARNING, " Field is full, person count: " + to_string(persons.size()));
  }
}

Person *PersonHandler::getReproductionPartner(const Person &person)
{
  // cout << "Get Reproduction Partner for Person Id: " << person.id << endl;
  vector<int> neighbIds = mapHandler.getPersonNeighbourIds(person);
  for (int neighbId : neighbIds)
  {
    Person *neighbourP = getPersonById(neighbId);
    if (neighbourP)
    {
      Person &neighbour = *neighbourP;
      if (isReproductionPossible(person, neighbour))
      {
        return neighbourP;
      }
    }
  }
  return nullptr;
}

void PersonHandler::reproduce(Person &p1, Person &p2)
{
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

  logger.log(LogLevel::DEBUG, "Unborn baby created, id: " + to_string(baby.id) + " birthdate: " + birthDate.toDateString() +
                                  " mother id: " +
                                  to_string(motherId) + " father id: " + to_string(fatherId));
}

void PersonHandler::processBirths()
{
  vector<Person> unbornBabies;
  for (Person &babie : babies)
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

bool PersonHandler::isReproductionPossible(const Person &p1, const Person &p2)
{
  // Different sex, female not pregnant, not related
  return p1.gender != p2.gender && !p1.pregnant && !p2.pregnant && !isRelated(p1, p2);
}

bool PersonHandler::isRelated(const Person &p1, const Person &p2, int curRelLevel)
{

  // Check if persons are parent and kid (checks should be valid event if no parents)
  if (p1.motherId == p2.id || p2.motherId == p1.id)
    return true;
  if (p1.fatherId == p2.id || p2.fatherId == p1.id)
    return true;

  // skip parent checks when no parents
  if (!(p1.motherId == -1 || p1.fatherId == -1 || p2.motherId == -1 || p2.fatherId == -1))
  {
    // Check if persons are siblings
    if (p1.motherId == p2.motherId || p1.fatherId == p2.fatherId)
      return true;
  }
  else
  {
    logger.log(LogLevel::INFO, "Skipped parents relation check Person id : " + to_string(p1.id) + " (Mother id: " + to_string(p1.motherId) + " Father id: " + to_string(p1.fatherId) + ")" + " Person id : " + to_string(p2.id) + " (Mother id: " + to_string(p2.motherId) + " Father id: " + to_string(p2.fatherId) + ")");
  }

  // for example 0 < 1
  if (curRelLevel < releationLevel)
  {
    curRelLevel++;

    Person *m1 = getPersonById(p1.motherId);
    Person *f1 = getPersonById(p1.fatherId);
    Person *m2 = getPersonById(p2.motherId);
    Person *f2 = getPersonById(p2.fatherId);

    if (m1 && isRelated(*m1, p2, curRelLevel))
      return true;

    if (f1 && isRelated(*f1, p2, curRelLevel))
      return true;

    if (m2 && isRelated(*m2, p1, curRelLevel))
      return true;

    if (f2 && isRelated(*f2, p1, curRelLevel))
      return true;
  }

  return false;
}

Person *PersonHandler::getPersonById(int id)
{
  auto personIt = std::find_if(persons.begin(), persons.end(), [&](const Person &p)
                               { return p.id == id; });

  if (personIt != persons.end())
  {
    // it -> object -> pointer
    return &*personIt;
  }
  return nullptr;
}
