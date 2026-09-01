#include "../headers/Person.h"
#include <cstdlib>

Person::Person(int id, Date birthdate, string name, Gender gender, int dyingAge)
    : Object(id),
      id(id),
      birthdate(birthdate),
      name(name),
      gender(gender),
      coordinate{0, 0},
      pregnant(false),
      murder(false),
      mother(nullptr),
      father(nullptr),
      movementPerHour(1),
      murderCount(0),
      babyCount(0),
      sexCount(0),
      babyBirthDate{0, 0, 0},
      pregnancyDate{0, 0, 0},
      dyingAge(dyingAge)

{
}
string Person::toString() const
{
  return "Person id: " + to_string(this->id) +
         " Birthdate: " + this->birthdate.toDateString() +
         " Gender: " + (this->gender == Gender::Male ? " M " : " F ") +
         " Name: " + this->name;
}