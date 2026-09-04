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
    int personId;
    Date birthdate;
    string name;
    Gender gender;
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

    Person(int personId, Date birthdate, string name, Gender gender, int dyingAge, int movementPerHour);
    string toString() const;
};
