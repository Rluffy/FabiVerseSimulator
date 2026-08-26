#include "../headers/Person.h"
#include <cstdlib>

Person::Person(int id, Date birthdate, string name, Gender gender)
    : id(id),
      birthdate(birthdate),
      name(name),
      gender(gender),
      coordinate{0, 0},
      pregnant(false),
      motherId(-1),
      fatherId(-1),
      babyBirthDate{0, 0, 0},
      pregnancyDate{0, 0, 0}

{
}
string Person::toString() const
{
  return "Person id: " + to_string(this->id) +
         " Birthdate: " + this->birthdate.toDateString() +
         " Gender: " + (this->gender == Gender::Male ? " M " : " F ") +
         " Name: " + this->name;
}