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
  logger.log(LogLevel::INFO, "[PREP] Prepare Persons");

  for (int i = 0; i < conf.startPersonCount; i++)
  {
    nextPersonId++;
    Person *p = new Person(nextPersonId, {1, 1, 1990}, to_string(nextPersonId), getRandomGender(), getRandomDyingAge());
    Coordinate *cord = mapHandler.getNextFreePostion();
    if (cord)
    {
      insertPerson(p, *cord);
      logger.log(LogLevel::DEBUG, "[PREP] Added " + p->toString());
    }
  }
}

void PersonHandler::simulatePersons()
{
  for (int i = 0; i < 24; i++)
  {
    for (int p = 0; p < persons.size(); p++)
    {
      Person *person = persons.at(p);

      // skip if persons alread set dead
      if (person->dead)
      {
        continue;
      }
      bool moved = mapHandler.movePerson(*person);
      if (!moved)
      {
        logger.log(LogLevel::DEBUG, "[MOVED] id: " + to_string(person->id) + " not moved");
      }

      // murders only kill not reproduce
      if (isMurder(*person))
      {
        Person *victimP = getVictim(*person);
        if (victimP)
        {
          killPerson(*person, *victimP);
        }
      }

      // Check for reproduction partner
      Person *repPartner = getReproductionPartner(*person);
      // Only reproduce if found
      if (repPartner)
      {
        reproduce(person, repPartner);
      }
      // check if persons die age
      if (isDying(*person))
      {
        dying(*person);
      }
    }
  }
  processDeads();
  processBirths();
}

void PersonHandler::insertPerson(Person *p1, Coordinate cord)
{
  mapHandler.insertPersonCoordinate(p1, cord);
  persons.push_back(p1);
}

void PersonHandler::babyBirth(Person *baby)
{
  Person *motherP = baby->mother;
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
        babyMurderText = " murder";
        baby->murder = true;
      }

      insertPerson(baby, birthLocation);
      logger.log(LogLevel::INFO, "[BIRTH] id: " + to_string(baby->id) + " birthdate: " + baby->birthdate.toDateString() + " mother id: " + to_string(baby->mother->id) + " father id: " + to_string(baby->father->id) + babyMurderText);
    }
    else
    {
      logger.log(LogLevel::WARNING, "[BIRTH] Field is full, person count: " + to_string(persons.size()));
      delete baby;
    }
  }
}

Person *PersonHandler::getReproductionPartner(const Person &person)
{
  // cout << "Get Reproduction Partner for Person Id: " << person.id << endl;
  vector<Person *> neighbs = mapHandler.getPersonNeighbours(person);
  for (auto neighb : neighbs)
  {
    Person &neighbour = *neighb;
    if (isReproductionPossible(person, neighbour))
    {
      return neighb;
    }
  }
  return nullptr;
}

Person *PersonHandler::getVictim(const Person &person)
{
  // cout << "Get Reproduction Partner for Person Id: " << person.id << endl;
  vector<Person *> victims = mapHandler.getPersonNeighbours(person);
  return victims.begin() != victims.end() ? *victims.begin() : nullptr;
}

void PersonHandler::reproduce(Person *p1, Person *p2)
{

  p1->sexCount++;
  p2->sexCount++;
  overallSexCount++;

  Date birthDate;

  Person *father;
  Person *mother;

  if (p1->gender == Gender::Female)
  {
    makePersonPregnant(*p1);
    birthDate = p1->babyBirthDate;
    mother = p1;
    father = p2;
  }
  else if (p2->gender == Gender::Female)
  {
    makePersonPregnant(*p2);
    birthDate = p2->babyBirthDate;
    mother = p2;
    father = p1;
  }

  // create baby
  // 1/2 for male or female
  Gender babyGender = getRandomGender();
  int dyingAge = getRandomDyingAge();
  Person *baby = new Person(++nextPersonId, birthDate, to_string(nextPersonId), babyGender, dyingAge);
  baby->father = father;
  baby->mother = mother;
  babies.push_back(baby);

  logger.log(LogLevel::INFO, "[SEX][PREGNANT] embryo id: " + to_string(baby->id) + " birthdate: " + birthDate.toDateString() +
                                 " mother id: " +
                                 to_string(mother->id) + " father id: " + to_string(father->id));
}

void PersonHandler::processBirths()
{
  vector<Person *> unbornBabies;
  for (Person *babie : babies)
  {
    bool birthTime = timeHandler.currentDate >= babie->birthdate;
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
  vector<Person *> undeadPersons;
  for (Person *person : persons)
  {
    if (person->dead)
    {
      deadPersons.push_back(person);
    }
    else
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
  if (isRelated(&p1, &p2))
  {
    logger.log(LogLevel::DEBUG, "[SEX][RELATED] id: " + to_string(p1.id) + " is releated to" + " id: " + to_string(p2.id));
    return false;
  }

  if (!isAdult(p1) || !isAdult(p2))
  {
    logger.log(LogLevel::DEBUG, "[SEX][UNDERAGE] id: " + to_string(p1.id) + " or id: " + to_string(p2.id) + " not adult");
    return false;
  }
  return p1.gender != p2.gender && !p1.pregnant && !p2.pregnant;
}

bool PersonHandler::isRelated(const Person *p1, const Person *p2, int curRelLevel)
{
  Person *m1 = p1->mother;
  Person *f1 = p1->father;
  Person *m2 = p2->mother;
  Person *f2 = p2->father;

  // Check if persons are parent and kid (checks should be valid event if no parents)
  if (m1 == p2 || m2 == p1)
    return true;
  if (f1 == p2 || f2 == p1)
    return true;

  // skip parent checks when no parents
  // either persons has parents or no one
  if (m1 && f1 && m2 && f2)
  {
    // Check if persons are siblings
    if (m1 == m2 || f1 == f2)
      return true;
  }
  else
  {
    logger.log(LogLevel::DEBUG, "[RELATED] Skipped for id: " + to_string(p1->id) + " and id: " + to_string(p2->id) + ", no parents");
  }

  // for example 0 < 1
  if (curRelLevel < conf.relLevel)
  {
    curRelLevel++;

    if (m1 && isRelated(m1, p2, curRelLevel))
      return true;

    if (f1 && isRelated(f1, p2, curRelLevel))
      return true;

    if (m2 && isRelated(m2, p1, curRelLevel))
      return true;

    if (f2 && isRelated(f2, p1, curRelLevel))
      return true;
  }

  return false;
}

vector<Person *>::iterator PersonHandler::getUnbornBabyItByMotherId(Person *mother)
{
  auto babyIt = std::find_if(babies.begin(), babies.end(), [&](const Person *p)
                             { return p->mother == mother; });

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
    auto babyIt = getUnbornBabyItByMotherId(&victim);
    if (babyIt != babies.end())
    {
      Person *pB = *babyIt;
      babyVictimText = " Unborn baby, id: " + to_string(pB->id) + " also killed";
      babies.erase(babyIt);
      overallDeathCount++;
      overallPersonsKilled++;
      delete pB;
    }
  }

  logger.log(LogLevel::INFO, "[DEATH][MURDER] id: " + to_string(murder.id) + " killed victim id: " +
                                 to_string(victim.id) + babyVictimText);
  murder.murderCount++;
  overallPersonsKilled++;

  dying(victim, false);
}

void PersonHandler::dying(Person &p1, bool printLog)
{
  p1.dead = true;
  mapHandler.removePersonCoordinate(p1);
  overallDeathCount++;

  if (printLog)
  {

    logger.log(LogLevel::INFO, "[DEATH] id: " + to_string(p1.id) + " died at age: " +
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

void PersonHandler::logPersonStatistics()
{
  logger.log(LogLevel::INFO, "[STATS] Current person count: " + to_string(persons.size()));
  logger.log(LogLevel::INFO, "[STATS] Overall persons killed count: " + to_string(overallPersonsKilled));
  logger.log(LogLevel::INFO, "[STATS] Overall babies born count: " + to_string(overallBabyCount));
  logger.log(LogLevel::INFO, "[STATS] Overall sex count: " + to_string(overallSexCount));
  logger.log(LogLevel::INFO, "[STATS] Overall persons lived count: " + to_string(overallBabyCount + conf.startPersonCount));
  logger.log(LogLevel::INFO, "[STATS] Overall persons died count: " + to_string(overallDeathCount));
}
