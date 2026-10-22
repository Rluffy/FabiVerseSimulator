#include "../headers/Object.h"

Object::Object(ObjectKind kind, int id, int movementPerHour)
    : kind(kind),
      id(id),
      movementPerHour(movementPerHour),
      coordinate({-1, -1})
{
}
