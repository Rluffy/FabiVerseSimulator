
#include "../headers/MapHandler.h"

using namespace std;


  MapHandler::MapHandler(MapConfig conf)
  :conf(conf)
  {
  }


Coordinate *MapHandler::getNextFreePostion()
{
  for (int x = 0; x < conf.worldLength; x++)
  {
    for (int y = 0; y < conf.worldWidth; y++)
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

void MapHandler::movePerson2(Person &p)
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
    if (xCordinate > conf.worldLength || xCordinate < 0)
    {
      xCordinate -= movPerhour * moveDirectionX;
    }
    int moveDirectionY = rand() % (max - min + 1) + min;
    yCordinate += movPerhour * moveDirectionY;
    if (yCordinate > conf.worldWidth || yCordinate < 0)
    {
      yCordinate -= movPerhour * moveDirectionY;
    }
    newCoord = {xCordinate, yCordinate};
  } while (newCoord == p.coordinate);

  // Only move person when cordinates are free
  if (objectCoordinates.find(newCoord) == objectCoordinates.end())
  {
    updatePersonCoordinate2(p, newCoord);
  }
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
    if (xCordinate > conf.worldLength || xCordinate < 0)
    {
      xCordinate -= movPerhour * moveDirectionX;
    }
    int moveDirectionY = rand() % (max - min + 1) + min;
    yCordinate += movPerhour * moveDirectionY;
    if (yCordinate > conf.worldWidth || yCordinate < 0)
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

void MapHandler::updatePersonCoordinate2(Person &p, Coordinate newCoord)
{
  objectCoordinates2.erase(p.coordinate);
  objectCoordinates2.insert({newCoord, &p});
  p.coordinate = newCoord;
}

void MapHandler::insertPersonCoordinate(Person &p, Coordinate newCoord)
{
  objectCoordinates.insert({newCoord, p.id});
  p.coordinate = newCoord;
}

void MapHandler::insertPersonCoordinate2(Person* p, Coordinate newCoord)
{
  objectCoordinates2.insert({newCoord, p});
  p->coordinate = newCoord;
}

void MapHandler::removePersonCoordinate(const Person &p)
{
  objectCoordinates.erase(p.coordinate);
}

void MapHandler::removePersonCoordinate2(const Person &p)
{
  objectCoordinates.erase(p.coordinate);
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

vector<Person*> MapHandler::getPersonNeighbours(const Person &person)
{
  vector<Person*> neighbPtrs;
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

      auto it = objectCoordinates2.find({xNextPos, yNextPos});
      // neighbour found
      if (it != objectCoordinates2.end() && it->second )
      {
        auto* neighbour = dynamic_cast<Person*>(it->second);
        if(neighbour){
          neighbPtrs.push_back(neighbour);

        }
    
      }
    }
  }
  return neighbPtrs;
}

