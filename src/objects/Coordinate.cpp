#include "../headers/Coordinate.h"



bool Coordinate::operator<(const Coordinate& other) const {
    if (x != other.x)
        return x < other.x;

    return y < other.y;
}

bool Coordinate::operator==(const Coordinate& other) const {
    return x == other.x && y == other.y;
}