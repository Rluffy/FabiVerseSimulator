#include <iostream>
#include <string>
#include "Date.h"
#include "Gender.h"
#include "Coordinate.h"
#include "Object.h"

#pragma once
using namespace std;

class Person : public Object
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

    Person *motherP;
    Person *father;

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
