
#include "../headers/MapHandler.h"

using namespace std;


  MapHandler::MapHandler(int worldLength, int worldWidth)
  :worldLength(worldLength),
  worldWidth(worldWidth)
  {
  }


Coordinate *MapHandler::getNextFreePostion()
{
  for (int x = 0; x < worldLength; x++)
  {
    for (int y = 0; y < worldWidth; y++)
    {
      Coordinate cord = {x, y};
      auto it = objectCoordinates.find(cord);
      // Free postion found
      if (it == objectCoordinates.end())
      {
        return new Coordinate{x, y};
      }
    }
  }
  return nullptr;
}

void MapHandler::movePerson(Person &p)
{
  int min = -1;
  int max = 1;
  int movPerhour = p.movementPerHour;

  int xCordinate = p.coordinate.x;
  int yCordinate = p.coordinate.y;

  Coordinate newCoord;
  // Some random movement must happen
  do
  {
    int moveDirectionX = rand() % (max - min + 1) + min;
    xCordinate += movPerhour * moveDirectionX;
    if (xCordinate > worldLength || xCordinate < 0)
    {
      xCordinate -= movPerhour * moveDirectionX;
    }
    int moveDirectionY = rand() % (max - min + 1) + min;
    yCordinate += movPerhour * moveDirectionY;
    if (yCordinate > worldWidth || yCordinate < 0)
    {
      yCordinate -= movPerhour * moveDirectionY;
    }
    newCoord = {xCordinate, yCordinate};
  } while (newCoord == p.coordinate);

  // Only move person when cordinates are free
  if (objectCoordinates.find(newCoord) == objectCoordinates.end())
  {
    updatePersonCoordinate(p, newCoord);
  }
}

void MapHandler::updatePersonCoordinate(Person &p, Coordinate newCoord)
{
  objectCoordinates.erase(p.coordinate);
  objectCoordinates.insert({newCoord, p.id});
  p.coordinate = newCoord;
}

void MapHandler::insertPersonCoordinate(Person &p, Coordinate newCoord)
{
  objectCoordinates.insert({newCoord, p.id});
  p.coordinate = newCoord;
}

vector<int> MapHandler::getPersonNeighbourIds(const Person &person)
{
  vector<int> neighbIds;
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

      auto it = objectCoordinates.find({xNextPos, yNextPos});
      // neighbour found
      if (it != objectCoordinates.end())
      {
        neighbIds.push_back(it->second);
      }
    }
  }
  return neighbIds;
}
