#pragma once

#include <random>

class Random
{
public:
    // Seeded from std::random_device
    Random();
    explicit Random(unsigned int seed);

    // Uniform integer in [low, high], both bounds included
    int range(int low, int high);
    // True with the given probability in percent (0-100)
    bool percent(int probability);

private:
    std::mt19937 engine;
};
