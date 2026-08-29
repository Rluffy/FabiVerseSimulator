#include "../headers/PersonHandler.h"
#include <algorithm>

PersonHandler::PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler, Logger &logger, PersonConfig conf)
    : mapHandler(mapHandler),
      timeHandler(timeHandler),
      logger(logger),
      conf(conf),
      nextPersonId(0),
      overallPersonsKilled(0),
      overallBabyCount(0),
      overallSexCount(0),
      overallDeathCount(0)

{
}

void PersonHandler::preparePersons()
{
  logger.log(LogLevel::INFO, "Prepare Persons");

  for (int i = 0; i < conf.startPersonCount; i++)
  {
    nextPersonId++;
    Person p(nextPersonId, {1, 1, 1980}, to_string(nextPersonId), getRandomGender(), getRandomDyingAge());
    Coordinate *cord = mapHandler.getNextFreePostion();
    if (cord)
    {
      insertPerson(p, *cord);
      logger.log(LogLevel::DEBUG, "Added " + p.toString());
    }
  }
}

void PersonHandler::simulatePersons()
{
  for (int i = 0; i < 24; i++)
  {
    for (int p = 0; p < persons.size(); p++)
    {
      Person &person = persons.at(p);

      // skip if persons alread set dead
      if (person.dead)
      {
        continue;
      }
      mapHandler.movePerson(person);

      // murders only kill not reproduce
      if (isMurder(person))
      {
        Person *victimP = getVictim(person);
        if (victimP)
        {
          killPerson(person, *victimP);
        }
      }

      // Check for reproduction partner
      Person *repPartner = getReproductionPartner(person);
      // Only reproduce if found
      if (repPartner)
      {
        reproduce(person, *repPartner);
      }
// check if persons die age
      if (isDying(person))
      {
        dying(person);
      }
    }
  }
  processDeads();
  processBirths();
}

void PersonHandler::insertPerson(Person &p1, Coordinate cord)
{
  mapHandler.insertPersonCoordinate(p1, cord);
  persons.push_back(p1);
}

void PersonHandler::babyBirth(Person &baby)
{
  Person *motherP = getPersonById(baby.motherId);
  if (motherP)
  {
    motherP->pregnant = false;

    // find free coordinates
    Coordinate *birthLocationP = mapHandler.getNextFreePostion();

    // Baby only born when free space found
    if (birthLocationP)
    {
      Coordinate birthLocation = *birthLocationP;
      delete birthLocationP;
      string babyMurderText = "";
      motherP->babyCount++;
      overallBabyCount++;

      // baby become a murder in future
      if (getRandomMurderPersonality())
      {
        babyMurderText = " will become a murder";
        baby.murder = true;
      }

      insertPerson(baby, birthLocation);
      logger.log(LogLevel::INFO, "Baby born, id: " + to_string(baby.id) + " birthdate: " + baby.birthdate.toDateString() + " mother id: " + to_string(baby.motherId) + " father id: " + to_string(baby.fatherId) + babyMurderText);
    }
    else
    {
      logger.log(LogLevel::WARNING, " Field is full, person count: " + to_string(persons.size()));
    }
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

Person *PersonHandler::getVictim(const Person &person)
{
  // cout << "Get Reproduction Partner for Person Id: " << person.id << endl;
  vector<int> victimIds = mapHandler.getPersonNeighbourIds(person);
  for (int victimId : victimIds)
  {
    Person *victimP = getPersonById(victimId);
    if (victimP)
    {
      return victimP;
    }
  }
  return nullptr;
}

void PersonHandler::reproduce(Person &p1, Person &p2)
{

  p1.sexCount++;
  p2.sexCount++;
  overallSexCount++;

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
  Gender babyGender = getRandomGender();
  int dyingAge = getRandomDyingAge();
  Person baby(++nextPersonId, birthDate, to_string(nextPersonId), babyGender, dyingAge);
  baby.fatherId = fatherId;
  baby.motherId = motherId;
  babies.push_back(baby);

  logger.log(LogLevel::INFO, "Unborn baby created, id: " + to_string(baby.id) + " birthdate: " + birthDate.toDateString() +
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

void PersonHandler::processDeads()
{
  vector<Person> undeadPersons;
  for (Person &person : persons)
  {
    if (!person.dead)
    {
      undeadPersons.push_back(person);
    }
  }

  persons = undeadPersons;
}

void PersonHandler::makePersonPregnant(Person &p1)
{
  // cout << "Make Person Id: " << p1.id << " Pregnant" << endl;
  p1.pregnant = true;
  p1.pregnancyDate = timeHandler.currentDate;
  p1.babyBirthDate = timeHandler.currentDate;
  p1.babyBirthDate.addMonths(conf.pregTimeMonths);
}

bool PersonHandler::isReproductionPossible(const Person &p1, const Person &p2)
{

  // Different sex, female not pregnant, not related
  if (isRelated(p1, p2))
  {
    logger.log(LogLevel::DEBUG, p1.toString() + " is releated to " + p2.toString());
    return false;
  }

  if (!isAdult(p1) || !isAdult(p2))
  {
    logger.log(LogLevel::DEBUG, p1.toString() + " or " + p2.toString() + " not adult");
    return false;
  }
  return p1.gender != p2.gender && !p1.pregnant && !p2.pregnant;
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
    logger.log(LogLevel::DEBUG, "Skipped parents relation check Person id : " + to_string(p1.id) + " (Mother id: " + to_string(p1.motherId) + " Father id: " + to_string(p1.fatherId) + ")" + " Person id : " + to_string(p2.id) + " (Mother id: " + to_string(p2.motherId) + " Father id: " + to_string(p2.fatherId) + ")");
  }

  // for example 0 < 1
  if (curRelLevel < conf.relLevel)
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

vector<Person>::iterator PersonHandler::getPersonItById(int id)
{
  auto personIt = std::find_if(persons.begin(), persons.end(), [&](const Person &p)
                               { return p.id == id; });

  return personIt;
}

vector<Person>::iterator PersonHandler::getUnbornBabyItByMotherId(int motherId)
{
  auto babyIt = std::find_if(babies.begin(), babies.end(), [&](const Person &p)
                             { return p.motherId == motherId; });

  return babyIt;
}

Gender PersonHandler::getRandomGender()
{
  // probabitly in % to be a male
  return (rand() % 100 + 1) <= conf.maleProbability ? Gender::Male : Gender::Female;
}

bool PersonHandler::isAdult(const Person &p1)
{
  Date adultDate = p1.birthdate;
  adultDate.addYears(conf.adultAgeYears);

  if (timeHandler.currentDate >= adultDate)
  {
    return true;
  }

  return false;
}

bool PersonHandler::getRandomMurderPersonality()
{
  // probabitly in % to be a murder in future
  return (rand() % 100 + 1) <= conf.murderProbability;
}

bool PersonHandler::isMurder(const Person &p1)
{
  // person is only murder when he is adult
  return p1.murder && isAdult(p1);
}

void PersonHandler::killPerson(Person &murder, Person &victim)
{
  string babyVictimText = "";

  if (victim.pregnant)
  {
    // also baby will die
    auto babyIt = getUnbornBabyItByMotherId(victim.id);
    if (babyIt != babies.end())
    {
      babyVictimText = " Unborn Baby, id " + to_string(babyIt->id) + " also killed";
      babies.erase(babyIt);
    }
  }

  logger.log(LogLevel::INFO, "Murder, id: " + to_string(murder.id) + " killed victim id: " +
                                 to_string(victim.id) + babyVictimText);
  murder.murderCount++;
  overallPersonsKilled++;
 
  dying(victim,false);
}

void PersonHandler::dying(Person &p1, bool printLog)
{
  p1.dead = true;
  mapHandler.removePersonCoordinate(p1);
  overallDeathCount++;

  if(printLog){

  logger.log(LogLevel::INFO, "Person, id: " + to_string(p1.id) + " died at age: " +
                                 to_string(p1.dyingAge));
  }                              
}

int PersonHandler::getRandomDyingAge()
{
  return conf.minDyingAge + rand() % (conf.maxDyingAge - conf.minDyingAge + 1);
}

bool ::PersonHandler::isDying(const Person &p1)
{
  Date dyingDate = p1.birthdate;
  dyingDate.addYears(p1.dyingAge);

  if (timeHandler.currentDate >= dyingDate)
  {
    return true;
  }

  return false;
}
