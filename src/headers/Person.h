#pragma once

#include <string>
#include "Gender.h"
#include "Object.h"

class Person : public Object
{
public:
    // Day values are days since 1970-01-01, see Date::toDayNumber()
    Person(int id, int birthDay, int adultDay, int dyingDay, int dyingAge, Gender gender, int movementPerHour, int lastEatenDay);
    std::string toString() const;

    int birthDay;
    int adultDay;
    int dyingDay;
    int dyingAge;
    int lastEatenDay;
    // Only meaningful while pregnant
    int babyBirthDay = 0;
    Gender gender;
    bool pregnant = false;
    bool murder = false;
    bool dead = false;

    Person *motherP = nullptr;
    Person *father = nullptr;
};
