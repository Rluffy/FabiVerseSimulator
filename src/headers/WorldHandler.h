#pragma once

#include "World.h"
#include "TimeHandler.h"
#include "MapHandler.h"
#include "PersonHandler.h"
#include "FoodHandler.h"
#include "Logger.h"
#include "../configs/Config.h"

class WorldHandler
{
public:
   WorldHandler(Config conf);
   void startSimulation();

private:
   void simulateDay();
   void prepareSimulation();
   PersonHandler ph;
   TimeHandler th;
   MapHandler mh;
   Logger logger;
   WorldConfig conf;
   FoodHandler fh;
};
