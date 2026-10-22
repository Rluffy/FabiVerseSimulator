#include "headers/WorldHandler.h"
#include "configs/ConfigLoader.h"

int main()
{
    // Paths are relative to src/, where the simulator is started from (see Makefile)
    WorldHandler wh(loadConfig("../config/config.xml"));
    wh.startSimulation();
    return 0;
}
