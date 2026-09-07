
#pragma once

#include "Coordinate.h"

class Object
{
public:
     Object(int id, int movementPerHour = 0);

     // for dynamic casting
     virtual ~Object() = default;
     int id;
     int movementPerHour;
     Coordinate coordinate;
};
