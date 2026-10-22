#pragma once

#include "Coordinate.h"

enum class ObjectKind
{
    Person,
    Food
};

class Object
{
public:
    Object(ObjectKind kind, int id, int movementPerHour = 0);

    // Virtual so that deleting through an Object pointer is always safe.
    virtual ~Object() = default;

    const ObjectKind kind;
    int id;
    int movementPerHour;
    Coordinate coordinate;
};
