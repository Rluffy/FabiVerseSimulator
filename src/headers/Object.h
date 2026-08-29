
#pragma once

#include "Coordinate.h"

class Object
{
public:
     int id;
     Coordinate coordinate;
     // for dynamic casting
     virtual ~Object() = default;
};
