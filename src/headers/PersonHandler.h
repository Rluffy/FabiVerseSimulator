#pragma once

#include <vector>
#include "Person.h"
#include "MapHandler.h"
#include "TimeHandler.h"
#include "../configs/PersonConfig.h"
#include "Logger.h"

class PersonHandler
{

public:
    PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler, Logger &logger, PersonConfig conf);

    void preparePersons();
    void simulatePersons();
    void logPersonStatistics();

private:
    vector<Person *> babiesP;
    vector<Person *> personsP;
    vector<Person *> deadPersonsP;

    int nextPersonId;
    int overallPersonsKilled;
    int overallBabyCount;
    int overallSexCount;
    int overallDeathCount;

    PersonConfig conf;
    MapHandler &mapHandler;
    TimeHandler &timeHandler;
    Logger &logger;

    void insertPerson(Person *pP, Coordinate cord);
    void babyBirth(Person *bP);
    Person *getReproductionPartner(const Person &p1);
    Person *getVictim(const Person &p1);
    std::vector<Person *>::iterator getUnbornBabyItByMother(const Person *person);
    Gender getRandomGender();
    void reproduce(Person &p1, Person &p2);
    void makePersonPregnant(Person &p);
    void processBirths();
    void processDeads();
    bool isReproductionPossible(const Person &p1, const Person &p2);
    bool isRelated(const Person *p1P, const Person *p2P, int relLevel = 0);
    bool isAdult(const Person &p);
    bool isMurder(const Person &p);
    bool isDying(const Person &p);
    void dying(Person &p1, bool printLog = true);
    bool getRandomMurderPersonality();
    int getRandomDyingAge();
    void killPerson(Person &murder, Person &victim);
};