
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

bool MapHandler::movePerson(Person &p)
{
  int min = -1;
  int max = 1;
  int movePerhour = p.movementPerHour;

  int xCordinate = p.coordinate.x;
  int yCordinate = p.coordinate.y;

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
  } while (newCoord == p.coordinate);

  // Only move person when cordinates are free
  if (oCoordinates.find(newCoord) == oCoordinates.end())
  {
    updatePersonCoordinate(p, newCoord);
    return true;
  }

  return false;
}

void MapHandler::updatePersonCoordinate(Person &p, Coordinate newCoord)
{
  oCoordinates.erase(p.coordinate);
  oCoordinates.insert({newCoord, &p});
  p.coordinate = newCoord;
}

void MapHandler::insertPersonCoordinate(Person *pP, Coordinate newCoord)
{
  oCoordinates.insert({newCoord, pP});
  pP->coordinate = newCoord;
}

void MapHandler::removePersonCoordinate(const Person &p)
{
  oCoordinates.erase(p.coordinate);
}

vector<Person *> MapHandler::getPersonNeighbours(const Person &person)
{
  vector<Person *> neighbPtrs;
  int xP = person.coordinate.x;
  int yP = person.coordinate.y;
  int xNextPos, yNextPos;

  for (int x = -1; x < 2; x++)
  {
    xNextPos = xP + x;
    for (int y = -1; y < 2; y++)
    {
      // Skip same position
      if (x == 0 && y == 0)
      {
        continue;
      }
      yNextPos = yP + y;

      auto it = oCoordinates.find({xNextPos, yNextPos});
      // neighbour found
      if (it != oCoordinates.end() && it->second)
      {
        Person *neighbour = dynamic_cast<Person *>(it->second);
        if (neighbour)
        {
          neighbPtrs.push_back(neighbour);
        }
      }
    }
  }
  return neighbPtrs;
}
