#include "../headers/PersonHandler.h"

#include <algorithm>

namespace
{
// Day number of the day a person reaches the given age
int dayAfterYears(int birthDay, int years)
{
    Date date = Date::fromDayNumber(birthDay);
    date.addYears(years);
    return date.toDayNumber();
}
}

PersonHandler::PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler, Logger &logger, Random &random, FoodHandler &foodHandler, PersonConfig conf)
    : conf(conf),
      mapHandler(mapHandler),
      timeHandler(timeHandler),
      logger(logger),
      random(random),
      foodHandler(foodHandler),
      debugEnabled(logger.isEnabled(LogLevel::DEBUG))
{
}

void PersonHandler::preparePersons()
{
  logger.log(LogLevel::INFO, "[PREP] Prepare Persons");

  const int birthDay = Date{1, 1, 1990}.toDayNumber();

  for (int i = 0; i < conf.startPersonCount; i++)
  {
    auto cord = mapHandler.findFreePosition();
    if (!cord)
    {
      break;
    }

    Person &person = createPerson(birthDay, randomGender(), timeHandler.currentDay);
    livingPersons.push_back(&person);
    mapHandler.placeObject(person, *cord);

    if (debugEnabled)
    {
      logger.log(LogLevel::DEBUG, "[PREP] Added " + person.toString());
    }
  }
}

void PersonHandler::simulateDay()
{
  for (int hour = 0; hour < 24; hour++)
  {
    for (Person *personP : livingPersons)
    {
      Person &person = *personP;

      // Can be killed earlier in this hour
      if (person.dead)
      {
        continue;
      }

      bool moved = mapHandler.moveObject(person);
      if (!moved && debugEnabled)
      {
        logger.log(LogLevel::DEBUG, "[MOVED] personId: " + std::to_string(person.id) + " not moved");
      }

      lookAround(person);

      if (isDying(person))
      {
        dying(person);
      }
      else if (isDyingStarving(person))
      {
        dyingStarving(person);
      }
    }
  }

  removeDeadFromLiving();
  processBirths();
}

int PersonHandler::getPersonsSize() const
{
  return static_cast<int>(livingPersons.size());
}

Person &PersonHandler::createPerson(int birthDay, Gender gender, int lastEatenDay)
{
  const int dyingAge = random.range(conf.minDyingAge, conf.maxDyingAge);

  allPersons.push_back(std::make_unique<Person>(
      ++nextPersonId,
      birthDay,
      dayAfterYears(birthDay, conf.adultAgeYears),
      dayAfterYears(birthDay, dyingAge),
      dyingAge,
      gender,
      conf.movementPerHour,
      lastEatenDay));

  return *allPersons.back();
}

Gender PersonHandler::randomGender()
{
  return random.percent(conf.maleProbability) ? Gender::Male : Gender::Female;
}

void PersonHandler::lookAround(Person &p)
{
  const auto neighbours = mapHandler.getNeighbours(p.coordinate);

  if (isMurder(p))
  {
    for (Object *o : neighbours)
    {
      if (o && o->kind == ObjectKind::Person)
      {
        auto *victimP = static_cast<Person *>(o);
        if (!victimP->dead)
        {
          killPerson(p, *victimP);
          break;
        }
      }
    }
  }

  if (isHungry(p))
  {
    for (Object *o : neighbours)
    {
      if (o && o->kind == ObjectKind::Food)
      {
        eat(p, *static_cast<Food *>(o));
        return;
      }
    }
  }

  for (Object *o : neighbours)
  {
    if (!o || o->kind != ObjectKind::Person)
    {
      continue;
    }

    auto *partnerP = static_cast<Person *>(o);
    if (!partnerP->dead && isReproductionPossible(p, *partnerP))
    {
      reproduce(p, *partnerP);
      return;
    }
  }
}

void PersonHandler::reproduce(Person &p1, Person &p2)
{
  bool p1IsMother = p1.gender == Gender::Female;
  Person &mother = p1IsMother ? p1 : p2;
  Person &father = p1IsMother ? p2 : p1;

  makePersonPregnant(mother);

  Person &baby = createPerson(mother.babyBirthDay, randomGender(), timeHandler.currentDay);
  baby.motherP = &mother;
  baby.father = &father;
  unbornPersons.push_back(&baby);

  logger.log(LogLevel::INFO, "[SEX][PREGNANT] embryo id: " + std::to_string(baby.id) +
                                 " birthdate: " + Date::fromDayNumber(baby.birthDay).toDateString() +
                                 " mother personId: " + std::to_string(mother.id) +
                                 " father personId: " + std::to_string(father.id));
  stats.sex++;
}

void PersonHandler::makePersonPregnant(Person &p)
{
  Date birthDate = Date::fromDayNumber(timeHandler.currentDay);
  birthDate.addMonths(conf.pregTimeMonths);

  p.pregnant = true;
  p.babyBirthDay = birthDate.toDayNumber();
}

void PersonHandler::processBirths()
{
  std::erase_if(unbornPersons, [this](Person *babyP)
                {
    if (babyP->birthDay > timeHandler.currentDay)
    {
      return false;
    }
    babyBirth(*babyP);
    return true; });
}

void PersonHandler::babyBirth(Person &baby)
{
  Person *motherP = baby.motherP;
  motherP->pregnant = false;

  auto cord = mapHandler.findFreePosition();
  if (!cord)
  {
    logger.log(LogLevel::WARNING, "[BIRTH] Field is full, person count: " + std::to_string(livingPersons.size()));
    baby.dead = true;
    return;
  }

  std::string murderText;
  if (random.percent(conf.murderProbability))
  {
    baby.murder = true;
    murderText = " murder";
  }

  livingPersons.push_back(&baby);
  mapHandler.placeObject(baby, *cord);
  stats.babies++;

  logger.log(LogLevel::INFO, "[BIRTH] personId: " + std::to_string(baby.id) +
                                 " birthdate: " + Date::fromDayNumber(baby.birthDay).toDateString() +
                                 " mother personId: " + std::to_string(motherP->id) +
                                 " father personId: " + std::to_string(baby.father->id) + murderText);
}

void PersonHandler::removeDeadFromLiving()
{
  std::erase_if(livingPersons, [](const Person *p)
                { return p->dead; });
}

bool PersonHandler::isReproductionPossible(const Person &p1, const Person &p2) const
{
  // Different sex, neither one pregnant
  if (p1.gender == p2.gender || p1.pregnant || p2.pregnant)
  {
    return false;
  }

  if (!isAdult(p1) || !isAdult(p2))
  {
    return false;
  }

  return !isRelated(&p1, &p2);
}

bool PersonHandler::isRelated(const Person *p1P, const Person *p2P, int curRelLevel) const
{
  const Person *m1P = p1P->motherP;
  const Person *f1P = p1P->father;
  const Person *m2P = p2P->motherP;
  const Person *f2P = p2P->father;

  // Parent and child (checked even when a parent is unknown)
  if (m1P == p2P || m2P == p1P)
    return true;
  if (f1P == p2P || f2P == p1P)
    return true;

  // Siblings, only when both parents are known
  if (m1P && f1P && m2P && f2P)
  {
    if (m1P == m2P || f1P == f2P)
      return true;
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

void PersonHandler::killPerson(Person &murder, Person &victim)
{
  std::string unbornText;

  if (victim.pregnant)
  {
    // The unborn baby dies with its mother
    auto babyIt = std::find_if(unbornPersons.begin(), unbornPersons.end(), [&victim](const Person *babyP)
                               { return babyP->motherP == &victim; });

    if (babyIt != unbornPersons.end())
    {
      Person *babyP = *babyIt;
      unbornPersons.erase(babyIt);
      babyP->dead = true;
      unbornText = " Unborn baby, personId: " + std::to_string(babyP->id) + " also killed";
      stats.deaths++;
      stats.killed++;
    }
  }

  logger.log(LogLevel::INFO, "[DEATH][MURDER] personId: " + std::to_string(murder.id) +
                                 " killed victim personId: " + std::to_string(victim.id) + unbornText);
  stats.killed++;

  dying(victim, false);
}

void PersonHandler::dyingStarving(Person &p)
{
  logger.log(LogLevel::INFO, "[DEATH][EAT] personId: " + std::to_string(p.id) +
                                 " starved last eaten: " + Date::fromDayNumber(p.lastEatenDay).toDateString());

  stats.starved++;
  dying(p, false);
}

void PersonHandler::dying(Person &p, bool printLog)
{
  p.dead = true;
  mapHandler.removeObject(p);

  if (printLog)
  {
    logger.log(LogLevel::INFO, "[DEATH] personId: " + std::to_string(p.id) +
                                   " died at age: " + std::to_string(p.dyingAge));
  }

  stats.deaths++;
}

bool PersonHandler::isAdult(const Person &p) const
{
  return timeHandler.currentDay >= p.adultDay;
}

bool PersonHandler::isMurder(const Person &p) const
{
  // A murder personality only acts once the person is an adult
  return p.murder && isAdult(p);
}

bool PersonHandler::isHungry(const Person &p) const
{
  return timeHandler.currentDay >= p.lastEatenDay + conf.hungryStartDays;
}

bool PersonHandler::isDying(const Person &p) const
{
  return timeHandler.currentDay >= p.dyingDay;
}

bool PersonHandler::isDyingStarving(const Person &p) const
{
  return timeHandler.currentDay >= p.lastEatenDay + conf.hungryDeathDays;
}

void PersonHandler::eat(Person &p, Food &food)
{
  foodHandler.consume(food);
  p.lastEatenDay = timeHandler.currentDay;

  if (debugEnabled)
  {
    logger.log(LogLevel::DEBUG, "[EAT] personId: " + std::to_string(p.id) + " eats food");
  }
}

void PersonHandler::logPersonStatistics() const
{
  logger.log(LogLevel::INFO, "[STATS] Current person count: " + std::to_string(livingPersons.size()));
  logger.log(LogLevel::INFO, "[STATS] Overall persons killed count: " + std::to_string(stats.killed));
  logger.log(LogLevel::INFO, "[STATS] Overall babies born count: " + std::to_string(stats.babies));
  logger.log(LogLevel::INFO, "[STATS] Overall sex count: " + std::to_string(stats.sex));
  logger.log(LogLevel::INFO, "[STATS] Overall persons lived count: " + std::to_string(stats.babies + conf.startPersonCount));
  logger.log(LogLevel::INFO, "[STATS] Overall persons died count: " + std::to_string(stats.deaths));
  logger.log(LogLevel::INFO, "[STATS] Overall persons starved count: " + std::to_string(stats.starved));
}
