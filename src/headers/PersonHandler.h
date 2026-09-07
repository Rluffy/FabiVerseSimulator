#pragma once

#include <vector>
#include "Person.h"
#include "MapHandler.h"
#include "TimeHandler.h"
#include "../configs/PersonConfig.h"
#include "Logger.h"
#include <concepts>
#include <algorithm>
#include "Food.h"

class PersonHandler
{

public:
    PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler, Logger &logger, PersonConfig conf);
    int getPersonsSize();
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
    int overallStarvedCount;

    PersonConfig conf;
    MapHandler &mapHandler;
    TimeHandler &timeHandler;
    Logger &logger;

    void insertPerson(Person *pP, Coordinate cord);
    void babyBirth(Person *bP);
    Person *getReproductionPartner(const Person &p, const vector<Object *> &neighbsP);
    Person *getVictim(const Person &p, vector<Object *> &neighsP);

    template <typename O>
    O *getFirstObject(vector<Object *> &objectsP, bool deleteObject = false)
    {
        auto oIt = std::find_if(objectsP.begin(), objectsP.end(), [&](Object *o)
                                { return dynamic_cast<O *>(o) != nullptr; });

        if (oIt != objectsP.end())
        {
            O *convOp = dynamic_cast<O *>(*oIt);
            if (deleteObject)
            {
                objectsP.erase(oIt);
            }
            return convOp;
        };

        return nullptr;
    }

    template <typename O>
    vector<O *> getSpecObjects(const vector<Object *> &objectsP)
    {
        vector<O *> convertObjsP;

        for (auto oP : objectsP)
        {
            auto convObjP = dynamic_cast<O *>(oP);
            if (convObjP)
            {
                convertObjsP.push_back(convObjP);
            }
        }

        return convertObjsP;
    };

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
    bool isDyingStarving(const Person &p);
    bool isHungry(const Person &p);
    void eat(Person &p, Food *food);
    void dying(Person &p1, bool printLog = true);
    bool getRandomMurderPersonality();
    int getRandomDyingAge();
    void killPerson(Person &murder, Person &victim);
    void dyingStarving(Person &p);
    void lookAround(Person &p);
};