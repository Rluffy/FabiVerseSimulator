#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Person.h"
#include "MapHandler.h"
#include "TimeHandler.h"
#include "Logger.h"
#include "Random.h"
#include "FoodHandler.h"
#include "../configs/PersonConfig.h"

class PersonHandler
{
public:
    PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler, Logger &logger, Random &random, FoodHandler &foodHandler, PersonConfig conf);
    void preparePersons();
    void simulateDay();
    int getPersonsSize() const;
    void logPersonStatistics() const;

private:
    struct Statistics
    {
        int killed = 0;
        int babies = 0;
        int sex = 0;
        int deaths = 0;
        int starved = 0;
    };

    // Owns every person, dead ones included, so parent pointers never dangle
    std::vector<std::unique_ptr<Person>> allPersons;
    std::vector<Person *> livingPersons;
    std::vector<Person *> unbornPersons;
    int nextPersonId = 0;
    Statistics stats;

    PersonConfig conf;
    MapHandler &mapHandler;
    TimeHandler &timeHandler;
    Logger &logger;
    Random &random;
    FoodHandler &foodHandler;
    bool debugEnabled;

    Person &createPerson(int birthDay, Gender gender, int lastEatenDay);
    Gender randomGender();
    void lookAround(Person &p);
    void reproduce(Person &p1, Person &p2);
    void makePersonPregnant(Person &p);
    void processBirths();
    void babyBirth(Person &baby);
    void removeDeadFromLiving();
    void killPerson(Person &murder, Person &victim);
    void dying(Person &p, bool printLog = true);
    void dyingStarving(Person &p);
    void eat(Person &p, Food &food);

    bool isReproductionPossible(const Person &p1, const Person &p2) const;
    bool isRelated(const Person *p1P, const Person *p2P, int curRelLevel = 0) const;
    bool isAdult(const Person &p) const;
    bool isHungry(const Person &p) const;
    bool isDying(const Person &p) const;
    bool isDyingStarving(const Person &p) const;
    bool isMurder(const Person &p) const;
};
