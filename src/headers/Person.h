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
    int motherId;
    int fatherId;
    Date babyBirthDate;
    Date pregnancyDate;
    int movementPerHour = 1;

    Person(int id, Date birthdate, string name, Gender gender);
     string toString() const;
};
