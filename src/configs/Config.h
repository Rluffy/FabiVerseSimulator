
#pragma once

#include "LogConfig.h"
#include "MapConfig.h"
#include "PersonConfig.h"
#include "TimeConfig.h"
#include "WorldConfig.h"
#include "FoodConfig.h"

struct Config
{
    LogConfig logConfig;
    MapConfig mapConfig;
    PersonConfig personConfig;
    TimeConfig timeConfig;
    WorldConfig worldConfig;
    FoodConfig foodConfig;
};