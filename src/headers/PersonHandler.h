#pragma once

#include <vector>
#include "Person.h"
#include "MapHandler.h"
#include "TimeHandler.h"
#include "../configs/PersonConfig.h"
#include "Logger.h"

class PersonHandler{

    public:
     vector<Person> persons;
     vector<Person> babies;
     int nextPersonId = 0;
     PersonConfig conf;
     MapHandler &mapHandler;
     TimeHandler &timeHandler;
     Logger &logger;
     PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler, Logger &logger, PersonConfig conf);
 
     void insertPerson(Person &p1, Coordinate cord);
     void preparePersons();
     void simulatePersons();
     void babyBirth(Person &baby);
     Person* getReproductionPartner(const Person &p1);
     Person* getPersonById(int id);
     Gender getRandomGender();
     void reproduce(Person &p1, Person &p2);
     void makePersonPregnant(Person & p1);
     void processBirths();
     bool isReproductionPossible(const Person &p1, const Person &p2);
     bool isRelated(const Person &p1, const Person &p2, int relLevel = 0);
     bool isAdult(const Person &p1);





};