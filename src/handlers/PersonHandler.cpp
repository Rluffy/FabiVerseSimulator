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
    Person *pP = new Person(nextPersonId, {1, 1, 1990}, to_string(nextPersonId), getRandomGender(), getRandomDyingAge());
    auto cord = mapHandler.getNextFreePostion();
    if (cord)
    {
      insertPerson(pP, *cord);
      logger.log(LogLevel::DEBUG, "[PREP] Added " + pP->toString());
    }
  }
}

void PersonHandler::simulatePersons()
{
  for (int i = 0; i < 24; i++)
  {
    for (auto personP : personsP)
    {
      Person &person = *personP;
      // skip if persons alread set dead
      if (person.dead)
      {
        continue;
      }
      bool moved = mapHandler.movePerson(person);
      if (!moved)
      {
        logger.log(LogLevel::DEBUG, "[MOVED] id: " + to_string(person.id) + " not moved");
      }
      if (isMurder(person))
      {
        Person *victimP = getVictim(person);
        if (victimP)
        {
          killPerson(person, *victimP);
        }
      }

      // Check for reproduction partner
      Person *repPartnerP = getReproductionPartner(person);
      // Only reproduce if found
      if (repPartnerP)
      {
        reproduce(person, *repPartnerP);
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

void PersonHandler::insertPerson(Person *pP, Coordinate cord)
{
  mapHandler.insertPersonCoordinate(pP, cord);
  personsP.push_back(pP);
}

void PersonHandler::babyBirth(Person *babyP)
{
  Person *motherP = babyP->motherP;
  if (motherP)
  {
    motherP->pregnant = false;

    // find free coordinates
    auto birthLocationO = mapHandler.getNextFreePostion();

    // Baby only born when free space found
    if (birthLocationO)
    {
      string babyMurderText = "";
      // baby become a murder in future
      if (getRandomMurderPersonality())
      {
        babyMurderText = " murder";
        babyP->murder = true;
      }

      insertPerson(babyP, *birthLocationO);

      motherP->babyCount++;
      overallBabyCount++;
      logger.log(LogLevel::INFO, "[BIRTH] id: " + to_string(babyP->id) + " birthdate: " + babyP->birthdate.toDateString() + " mother id: " + to_string(babyP->motherP->id) + " father id: " + to_string(babyP->father->id) + babyMurderText);
    }
    else
    {
      logger.log(LogLevel::WARNING, "[BIRTH] Field is full, person count: " + to_string(personsP.size()));
      delete babyP;
    }
  }
}

Person *PersonHandler::getReproductionPartner(const Person &p)
{
  vector<Person *> neighbsP = mapHandler.getPersonNeighbours(p);
  for (auto neighbP : neighbsP)
  {
    if (isReproductionPossible(p, *neighbP))
    {
      return neighbP;
    }
  }
  return nullptr;
}

Person *PersonHandler::getVictim(const Person &p)
{
  vector<Person *> victims = mapHandler.getPersonNeighbours(p);

  if (!victims.empty())
  {
    return *victims.begin();
  }
  return nullptr;
}

void PersonHandler::reproduce(Person &p1, Person &p2)
{
  Date birthDate;

  Person *fatherP;
  Person *motherP;

  if (p1.gender == Gender::Female)
  {
    makePersonPregnant(p1);
    birthDate = p1.babyBirthDate;
    motherP = &p1;
    fatherP = &p2;
  }
  else if (p2.gender == Gender::Female)
  {
    makePersonPregnant(p2);
    birthDate = p2.babyBirthDate;
    motherP = &p2;
    fatherP = &p1;
  }

  // create baby
  Gender babyGender = getRandomGender();
  int dyingAge = getRandomDyingAge();
  Person *baby = new Person(++nextPersonId, birthDate, to_string(nextPersonId), babyGender, dyingAge);
  baby->father = fatherP;
  baby->motherP = motherP;
  babiesP.push_back(baby);

  logger.log(LogLevel::INFO, "[SEX][PREGNANT] embryo id: " + to_string(baby->id) + " birthdate: " + birthDate.toDateString() +
                                 " mother id: " +
                                 to_string(motherP->id) + " father id: " + to_string(fatherP->id));
  p1.sexCount++;
  p2.sexCount++;
  overallSexCount++;
}

void PersonHandler::processBirths()
{
  vector<Person *> unbornBabiesP;
  for (auto babieP : babiesP)
  {
    bool birthTime = timeHandler.currentDate >= babieP->birthdate;
    if (birthTime)
    {
      babyBirth(babieP);
    }
    else
    {
      unbornBabiesP.push_back(babieP);
    }
  }
  babiesP = unbornBabiesP;
}

void PersonHandler::processDeads()
{
  vector<Person *> undeadPersonsP;
  for (Person *personP : personsP)
  {
    if (personP->dead)
    {
      deadPersonsP.push_back(personP);
    }
    else
    {
      undeadPersonsP.push_back(personP);
    }
  }

  personsP = undeadPersonsP;
}

void PersonHandler::makePersonPregnant(Person &p)
{
  p.pregnant = true;
  p.pregnancyDate = timeHandler.currentDate;
  p.babyBirthDate = timeHandler.currentDate;
  p.babyBirthDate.addMonths(conf.pregTimeMonths);
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

bool PersonHandler::isRelated(const Person *p1P, const Person *p2P, int curRelLevel)
{
  Person *m1P = p1P->motherP;
  Person *f1P = p1P->father;
  Person *m2P = p2P->motherP;
  Person *f2P = p2P->father;

  // Check if persons are parent and kid (checks should be valid event if no parents)
  if (m1P == p2P || m2P == p1P)
    return true;
  if (f1P == p2P || f2P == p1P)
    return true;

  // skip parent checks when no parents
  // either persons has parents or no one
  if (m1P && f1P && m2P && f2P)
  {
    // Check if persons are siblings
    if (m1P == m2P || f1P == f2P)
      return true;
  }
  else
  {
    logger.log(LogLevel::DEBUG, "[RELATED] Skipped for id: " + to_string(p1P->id) + " and id: " + to_string(p2P->id) + ", no parents");
  }

  // for example 0 < 1
  if (curRelLevel < conf.relLevel)
  {
    curRelLevel++;

    if (m1P && isRelated(m1P, p2P, curRelLevel))
      return true;

    if (f1P && isRelated(f1P, p2P, curRelLevel))
      return true;

    if (m2P && isRelated(m2P, p1P, curRelLevel))
      return true;

    if (f2P && isRelated(f2P, p1P, curRelLevel))
      return true;
  }

  return false;
}

vector<Person *>::iterator PersonHandler::getUnbornBabyItByMother(const Person *mP)
{
  auto babyIt = std::find_if(babiesP.begin(), babiesP.end(), [&](const Person *p)
                             { return p->motherP == mP; });

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
    auto babyIt = getUnbornBabyItByMother(&victim);
    if (babyIt != babiesP.end())
    {
      Person *pB = *babyIt;
      babyVictimText = " Unborn baby, id: " + to_string(pB->id) + " also killed";
      babiesP.erase(babyIt);
      delete pB;
      overallDeathCount++;
      overallPersonsKilled++;
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
  if (printLog)
  {

    logger.log(LogLevel::INFO, "[DEATH] id: " + to_string(p1.id) + " died at age: " +
                                   to_string(p1.dyingAge));
  }
  overallDeathCount++;
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
  logger.log(LogLevel::INFO, "[STATS] Current person count: " + to_string(personsP.size()));
  logger.log(LogLevel::INFO, "[STATS] Overall persons killed count: " + to_string(overallPersonsKilled));
  logger.log(LogLevel::INFO, "[STATS] Overall babies born count: " + to_string(overallBabyCount));
  logger.log(LogLevel::INFO, "[STATS] Overall sex count: " + to_string(overallSexCount));
  logger.log(LogLevel::INFO, "[STATS] Overall persons lived count: " + to_string(overallBabyCount + conf.startPersonCount));
  logger.log(LogLevel::INFO, "[STATS] Overall persons died count: " + to_string(overallDeathCount));
}
