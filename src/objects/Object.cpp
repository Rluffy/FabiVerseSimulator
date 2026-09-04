#include "../headers/Object.h"

Object::Object(int id, int movementPerHour)
    : id(id),
      movementPerHour(movementPerHour),
      coordinate({-1, -1})
{
}