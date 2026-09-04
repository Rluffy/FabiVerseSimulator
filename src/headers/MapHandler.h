#pragma once

#include "Coordinate.h"
#include "Person.h"
#include "Object.h"
#include <map>
#include <vector>
#include "../configs/MapConfig.h"
#include <optional>
#include <concepts>

class MapHandler
{
public:
   MapHandler(MapConfig conf);
   bool moveObject(Object &o);
   void updateObjectCoordinate(Object &o, Coordinate newCoord);
   void insertObjectCoordinate(Object *oP, Coordinate newCoord);
   void removeObjectCoordinate(const Object &o);
   std::optional<Coordinate> getNextFreePostion();

   template <typename O>
   vector<O *> getObjectNeighbours(const Object &o)
   {
      vector<O *> objectsP;

      int xP = o.coordinate.x;
      int yP = o.coordinate.y;
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
               O *neighbour = dynamic_cast<O *>(it->second);
               if (neighbour)
               {
                  objectsP.push_back(neighbour);
               }
            }
         }
      }
      return objectsP;
   };

   MapConfig conf;

private:
   std::map<Coordinate, Object *> oCoordinates;
};