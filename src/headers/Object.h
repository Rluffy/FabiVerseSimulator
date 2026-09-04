
#pragma once

#include "Coordinate.h"

class Object
{
public:
     Object(int id, int movementPerHour = 0);

     // for dynamic casting
     virtual ~Object() = default;

private:
     int id;
     int movementPerHour;
     Coordinate coordinate;
};
