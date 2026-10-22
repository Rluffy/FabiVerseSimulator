#include "../headers/MapHandler.h"

#include <algorithm>

MapHandler::MapHandler(MapConfig conf, Random &random)
    : conf(conf),
      random(random),
      cells(static_cast<size_t>(conf.worldLength) * conf.worldWidth, nullptr)
{
}

size_t MapHandler::indexOf(Coordinate c) const
{
  return static_cast<size_t>(c.x) * conf.worldWidth + c.y;
}

bool MapHandler::isInside(int x, int y) const
{
  return x >= 0 && x < conf.worldLength && y >= 0 && y < conf.worldWidth;
}

std::optional<Coordinate> MapHandler::findFreePosition()
{
  while (firstFreeIndex < cells.size() && cells[firstFreeIndex] != nullptr)
  {
    firstFreeIndex++;
  }

  if (firstFreeIndex == cells.size())
  {
    return std::nullopt;
  }

  auto width = static_cast<size_t>(conf.worldWidth);
  return Coordinate{static_cast<int>(firstFreeIndex / width), static_cast<int>(firstFreeIndex % width)};
}

bool MapHandler::moveObject(Object &o)
{
  int step = o.movementPerHour;
  Coordinate target = {
      o.coordinate.x + random.range(-1, 1) * step,
      o.coordinate.y + random.range(-1, 1) * step};

  // An axis that would leave the map keeps its old value
  if (target.x < 0 || target.x >= conf.worldLength)
  {
    target.x = o.coordinate.x;
  }
  if (target.y < 0 || target.y >= conf.worldWidth)
  {
    target.y = o.coordinate.y;
  }

  if (target == o.coordinate || cells[indexOf(target)] != nullptr)
  {
    return false;
  }

  removeObject(o);
  placeObject(o, target);
  return true;
}

void MapHandler::placeObject(Object &o, Coordinate newCoord)
{
  cells[indexOf(newCoord)] = &o;
  o.coordinate = newCoord;
}

void MapHandler::removeObject(const Object &o)
{
  size_t index = indexOf(o.coordinate);
  cells[index] = nullptr;
  firstFreeIndex = std::min(firstFreeIndex, index);
}

std::array<Object *, 8> MapHandler::getNeighbours(Coordinate c) const
{
  std::array<Object *, 8> neighbours{};
  size_t count = 0;

  for (int dx = -1; dx <= 1; dx++)
  {
    for (int dy = -1; dy <= 1; dy++)
    {
      if (dx == 0 && dy == 0)
      {
        continue;
      }

      int x = c.x + dx;
      int y = c.y + dy;
      if (isInside(x, y))
      {
        neighbours[count] = cells[indexOf({x, y})];
      }
      count++;
    }
  }

  return neighbours;
}
