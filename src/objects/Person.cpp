#include "../headers/Person.h"
#include <cstdlib>

Person::Person(int id, Date birthdate, string name, Gender gender)
    : id(id),
      birthdate(birthdate),
      name(name),
      gender(gender)

{

}