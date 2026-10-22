#include "../headers/FoodHandler.h"

FoodHandler::FoodHandler(FoodConfig conf, MapHandler &mapHandler)
    : conf(conf),
      mapHandler(mapHandler)
{
}

void FoodHandler::spawnFood()
{
    for (int i = 0; i < conf.foodSpawnRateDay; i++)
    {
        auto cord = mapHandler.findFreePosition();

        // Map is full, no more food fits in
        if (!cord)
        {
            break;
        }

        Food *food = new Food();
        mapHandler.placeObject(*food, *cord);
    }
}

void FoodHandler::consume(Food &food)
{
    mapHandler.removeObject(food);
    delete &food;
}
