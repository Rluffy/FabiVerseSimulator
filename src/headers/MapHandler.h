#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <vector>
#include "Coordinate.h"
#include "Object.h"
#include "Random.h"
#include "../configs/MapConfig.h"

class MapHandler
{
public:
   MapHandler(MapConfig conf, Random &random);

   // First free cell in row-major order, nothing when the map is full
   std::optional<Coordinate> findFreePosition();
   // One random step of movementPerHour cells. Moves only onto a free cell.
   bool moveObject(Object &o);
   void placeObject(Object &o, Coordinate newCoord);
   void removeObject(const Object &o);
   // The eight surrounding cells, nullptr where there is no object or no cell
   std::array<Object *, 8> getNeighbours(Coordinate c) const;

   MapConfig conf;

private:
   size_t indexOf(Coordinate c) const;
   bool isInside(int x, int y) const;

   Random &random;
   std::vector<Object *> cells;
   // All cells before this index are occupied, so the free search starts here
   size_t firstFreeIndex = 0;
};
