#include "../headers/Coordinate.h"

bool Coordinate::operator==(const Coordinate &other) const
{
    return x == other.x && y == other.y;
}
