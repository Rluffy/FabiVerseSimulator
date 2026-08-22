#pragma once

#include "Coordinate.h"
#include "Person.h"
#include <map>
#include <vector>


class MapHandler{
public:
   int worldLength;
   int worldWidth;
   std::map<Coordinate, int> objectCoordinates;

   MapHandler(int worldLength, int worldWidth);
   void movePerson(Person &p);
   void updatePersonCoordinate(Person &p, Coordinate newCoord);
   void insertPersonCoordinate(Person &p, Coordinate newCoord);

   Coordinate* getNextFreePostion();
   Person* getFirstNeighbour(const Person &person);
   vector<int> getPersonNeighbourIds(const Person &person);


   

};