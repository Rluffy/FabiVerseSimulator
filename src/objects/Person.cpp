#include "../headers/Person.h"
#include <cstdlib>

Person::Person(int personId, Date birthdate, string name, Gender gender, int dyingAge, int movementPerHour)
    : Object(personId, movementPerHour),
      personId(personId),
      birthdate(birthdate),
      name(name),
      gender(gender),
      pregnant(false),
      murder(false),
      motherP(nullptr),
      father(nullptr),
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
  return "Person personId: " + to_string(this->personId) +
         " Birthdate: " + this->birthdate.toDateString() +
         " Gender: " + (this->gender == Gender::Male ? " M " : " F ") +
         " Name: " + this->name;
}