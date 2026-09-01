
#pragma once

#include "Coordinate.h"

class Object
{
public:
     int id;
     Coordinate coordinate;

     Object(int id);
    
     // for dynamic casting
     virtual ~Object() = default;
};
