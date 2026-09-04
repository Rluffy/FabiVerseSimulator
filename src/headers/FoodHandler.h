#pragma once

#include "MapHandler.h"
#include "../configs/FoodConfig.h"
#include "Food.h"
#include <vector>

class FoodHandler
{

public:
    FoodHandler(FoodConfig conf, MapHandler MapHandler);
    void spawnFood();

private:
    FoodConfig conf;
    vector<Food*> foodsP;
    MapHandler &mapHandler;
};