#include "headers/WorldHandler.h"

int main(int argc, char const *argv[])
{
    World world (0, "World1", {1,1,2020}, 100, 100,36);
    WorldHandler wh(world);
    wh.startSimulation();
    return 0;
}
