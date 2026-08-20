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
    const int id;
    const Date birthdate;
    const string name;
    Gender gender;

     Coordinate coordinate;
     bool pregnant;
     Date babyBirthDate;
     Date pregnancyDate;
     int movementPerHour = 1;

    Person(int id, Date birthdate, string name, Gender gender, Coordinate coordinate);
    Coordinate move(int worldLength, int worldWidth);
  


};
