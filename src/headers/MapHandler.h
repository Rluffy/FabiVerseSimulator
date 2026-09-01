#pragma once

#include "Coordinate.h"
#include "Person.h"
#include "Object.h"
#include <map>
#include <vector>
#include "../configs/MapConfig.h"


class MapHandler{
public:
   std::map<Coordinate, int> objectCoordinates;
   std::map<Coordinate, Object*> objectCoordinates2;

   MapHandler(MapConfig conf);
   bool movePerson(Person &p);
   void updatePersonCoordinate(Person &p, Coordinate newCoord);
   void insertPersonCoordinate(Person* p, Coordinate newCoord);
   void removePersonCoordinate(const Person &p);
   Coordinate* getNextFreePostion();
   vector<Person*> getPersonNeighbours(const Person &person);
   MapConfig conf;


   

};