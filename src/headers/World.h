#pragma once

#include <iostream>
#include <string>
#include "Date.h"
#include "Gender.h"
#include "Person.h"
#include <vector>
#include <map>
#include "Person.h"


using namespace std;

class World
{
public:
   // Fix data
    const int id;
    const string name;
    //beginning of the world
    const Date startDate;
    const int length;
    const int width;

    const int pregnancyTimeMonths = 9;

    // Movement data
     Date currentDate;
     double temperature;
     vector<Person> persons;

     // BUG use references not copies for person
     map<Coordinate,Person*> objectCoordinates;
     int nextPersonId = 0;


     World (int id, string name, Date startDate, int length, int width);
     Person* getReproductionPartner(const Person& person);
     bool reproductionPossible(const Person& p1, const Person& p2 );
     void reproduce(Person& p1, Person& p2 );
     void babyBirth(Person& p1);
     Coordinate* getNextFreePostion();

     void startSimulation();
     void simulateDay();
     void prepareSimulation();
     


};
