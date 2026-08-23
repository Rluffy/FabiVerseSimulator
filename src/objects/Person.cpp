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