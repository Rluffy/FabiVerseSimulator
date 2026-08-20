#include <iostream>
#include "headers/World.h"
#include "headers/Coordinate.h"

using namespace std;

int main() {
World world(
    1,
    "FabiVerse",
    {1, 1, 2026},
    100,
    100
);

world.startSimulation();


  
}