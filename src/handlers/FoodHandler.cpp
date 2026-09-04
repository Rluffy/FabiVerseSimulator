#include "../headers/FoodHandler.h"

FoodHandler::FoodHandler(FoodConfig conf, MapHandler mapHandler)
    : conf(conf),
      mapHandler(mapHandler)
{
}

void FoodHandler::spawnFood()
{
    for (int i = 0; i < conf.foodSpawnRateDay; i++)
    {
        auto cordO = mapHandler.getNextFreePostion();

        if (cordO)
        {
            Food* fP = new Food("food");
            foodsP.push_back(fP);
            mapHandler.insertObjectCoordinate(fP,*cordO);
        }
        else
        {
            
        }
    }
}