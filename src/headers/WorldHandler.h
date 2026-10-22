#pragma once

#include "World.h"
#include "TimeHandler.h"
#include "MapHandler.h"
#include "PersonHandler.h"
#include "FoodHandler.h"
#include "Logger.h"
#include "Random.h"
#include "../configs/Config.h"

class WorldHandler
{
public:
   explicit WorldHandler(Config conf);
   void startSimulation();

private:
   void simulateDay();
   void prepareSimulation();

   // Members are initialised in declaration order: every dependency comes before its users
   Random random;
   TimeHandler th;
   MapHandler mh;
   Logger logger;
   FoodHandler fh;
   PersonHandler ph;
   WorldConfig conf;
};
