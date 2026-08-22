#pragma once

#include "World.h"
#include "TimeHandler.h"
#include "MapHandler.h"
#include "PersonHandler.h"

class WorldHandler
{
public:
   World world;
   WorldHandler(World world);
   void startSimulation();
private:
   void simulateDay();
   void prepareSimulation();
   PersonHandler ph;
   TimeHandler th;
   MapHandler mh;

};
