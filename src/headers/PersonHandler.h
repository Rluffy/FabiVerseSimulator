#pragma once

#include <vector>
#include "Person.h"
#include "MapHandler.h"
#include "TimeHandler.h"

class PersonHandler{

    public:
     vector<Person> persons;
     int nextPersonId = 0;
     MapHandler &mapHandler;
     TimeHandler &timeHandler;
     PersonHandler(MapHandler &mapHandler, TimeHandler &timeHandler);

     void preparePersons();
     void simulatePersons();
     void babyBirth(Person &p1);
     Person* getReproductionPartner(const Person &p1);
     void reproduce(Person &p1, Person &p2);
     void setPersonPregnant(Person & p1);








};