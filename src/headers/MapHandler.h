#pragma once

#include "Coordinate.h"
#include "Person.h"
#include <map>
#include <vector>
#include "../configs/MapConfig.h"


class MapHandler{
public:
   std::map<Coordinate, int> objectCoordinates;

   MapHandler(MapConfig conf);
   void movePerson(Person &p);
   void updatePersonCoordinate(Person &p, Coordinate newCoord);
   void insertPersonCoordinate(Person &p, Coordinate newCoord);
   void removePersonCoordinate(const Person &p);
   Coordinate* getNextFreePostion();
   Person* getFirstNeighbour(const Person &person);
   vector<int> getPersonNeighbourIds(const Person &person);
   MapConfig conf;


   

};