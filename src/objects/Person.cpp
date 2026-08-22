#include "../headers/Person.h"
#include <cstdlib>

Person::Person(int id, Date birthdate, string name, Gender gender)
    : id(id),
      birthdate(birthdate),
      name(name),
      gender(gender)

{

}

bool Person::reproductionPossible(const Person &p1, const Person &p2)
{
    // Different sex and female not pregnant
    return p1.gender != p2.gender && !p1.pregnant && !p2.pregnant;
}