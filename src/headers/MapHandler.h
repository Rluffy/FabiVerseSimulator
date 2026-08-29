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
   void movePerson(Person &p);
   void movePerson2(Person &p);
   void updatePersonCoordinate(Person &p, Coordinate newCoord);
   void updatePersonCoordinate2(Person &p, Coordinate newCoord);
   void insertPersonCoordinate(Person &p, Coordinate newCoord);
   void insertPersonCoordinate2(Person* p, Coordinate newCoord);
   void removePersonCoordinate(const Person &p);
   void removePersonCoordinate2(const Person &p);
   Coordinate* getNextFreePostion();
   vector<int> getPersonNeighbourIds(const Person &person);
   vector<Person*> getPersonNeighbours(const Person &person);
   MapConfig conf;


   

};