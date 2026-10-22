#pragma once

#include "MapHandler.h"
#include "Food.h"
#include "../configs/FoodConfig.h"

class FoodHandler
{
public:
    FoodHandler(FoodConfig conf, MapHandler &mapHandler);
    void spawnFood();
    // Removes eaten food from the map and frees it
    void consume(Food &food);

private:
    FoodConfig conf;
    MapHandler &mapHandler;
};
