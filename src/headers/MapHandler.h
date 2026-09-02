#pragma once

#include "Coordinate.h"
#include "Person.h"
#include "Object.h"
#include <map>
#include <vector>
#include "../configs/MapConfig.h"
#include <optional>

class MapHandler
{
public:
   MapHandler(MapConfig conf);
   bool movePerson(Person &p);
   void updatePersonCoordinate(Person &p, Coordinate newCoord);
   void insertPersonCoordinate(Person *pP, Coordinate newCoord);
   void removePersonCoordinate(const Person &p);
   std::optional<Coordinate> getNextFreePostion();
   vector<Person *> getPersonNeighbours(const Person &p);
   MapConfig conf;

private:
   std::map<Coordinate, Object *> oCoordinates;
};