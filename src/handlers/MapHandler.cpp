
#include "../headers/MapHandler.h"

using namespace std;

MapHandler::MapHandler(MapConfig conf)
    : conf(conf)
{
}

optional<Coordinate> MapHandler::getNextFreePostion()
{
  for (int x = 0; x < conf.worldLength; x++)
  {
    for (int y = 0; y < conf.worldWidth; y++)
    {
      Coordinate cord = {x, y};
      auto it = oCoordinates.find(cord);
      // Free postion found
      if (it == oCoordinates.end())
      {
        return cord;
      }
    }
  }
  return nullopt;
}

bool MapHandler::moveObject(Object &o)
{
  int min = -1;
  int max = 1;
  int movePerhour = o.movementPerHour;

  int xCordinate = o.coordinate.x;
  int yCordinate = o.coordinate.y;

  Coordinate newCoord;
  // Some random movement must happen
  do
  {
    int moveDirectionX = rand() % (max - min + 1) + min;
    int moveDirectionY = rand() % (max - min + 1) + min;

    xCordinate += movePerhour * moveDirectionX;
    yCordinate += movePerhour * moveDirectionY;

    if (xCordinate > conf.worldLength || xCordinate < 0)
    {
      xCordinate -= movePerhour * moveDirectionX;
    }

    if (yCordinate > conf.worldWidth || yCordinate < 0)
    {
      yCordinate -= movePerhour * moveDirectionY;
    }

    newCoord = {xCordinate, yCordinate};
  } while (newCoord == o.coordinate);

  // Only move person when cordinates are free
  if (oCoordinates.find(newCoord) == oCoordinates.end())
  {
    updateObjectCoordinate(o, newCoord);
    return true;
  }

  return false;
}

void MapHandler::updateObjectCoordinate(Object &o, Coordinate newCoord)
{
  oCoordinates.erase(o.coordinate);
  oCoordinates.insert({newCoord, &o});
  o.coordinate = newCoord;
}

void MapHandler::insertObjectCoordinate(Object *oP, Coordinate newCoord)
{
  oCoordinates.insert({newCoord, oP});
  oP->coordinate = newCoord;
}

void MapHandler::removeObjectCoordinate(const Object &o)
{
  oCoordinates.erase(o.coordinate);
}


