#pragma once

#include <map>

struct Coordinate
{
    int x;
    int y;
    bool operator<(const Coordinate& other) const;
    bool operator==(const Coordinate& other) const;
};