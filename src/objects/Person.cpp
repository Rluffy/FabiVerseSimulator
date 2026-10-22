#include "../headers/Person.h"
#include "../headers/Date.h"

Person::Person(int id, int birthDay, int adultDay, int dyingDay, int dyingAge, Gender gender, int movementPerHour, int lastEatenDay)
    : Object(ObjectKind::Person, id, movementPerHour),
      birthDay(birthDay),
      adultDay(adultDay),
      dyingDay(dyingDay),
      dyingAge(dyingAge),
      lastEatenDay(lastEatenDay),
      gender(gender)
{
}

std::string Person::toString() const
{
    return "Person personId: " + std::to_string(id) +
           " Birthdate: " + Date::fromDayNumber(birthDay).toDateString() +
           " Gender: " + (gender == Gender::Male ? " M " : " F ");
}
