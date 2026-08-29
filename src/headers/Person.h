#include <iostream>
#include <string>
#include "Date.h"
#include "Gender.h"
#include "Coordinate.h"

#pragma once
using namespace std;

class Person
{
public:
    int id;
    Date birthdate;
    string name;
    Gender gender;

    Coordinate coordinate;
    bool pregnant;
    bool murder;
    bool dead;

    
    int motherId;
    int fatherId;

    int murderCount;
    int babyCount;
    int sexCount;
    int dyingAge;
    Date babyBirthDate;
    Date pregnancyDate;
    int movementPerHour;

    Person(int id, Date birthdate, string name, Gender gender, int dyingAge);
     string toString() const;
};
