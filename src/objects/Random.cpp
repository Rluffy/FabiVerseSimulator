#include "../headers/Random.h"

Random::Random()
    : engine(std::random_device{}())
{
}

Random::Random(unsigned int seed)
    : engine(seed)
{
}

int Random::range(int low, int high)
{
    return std::uniform_int_distribution<int>(low, high)(engine);
}

bool Random::percent(int probability)
{
    return range(1, 100) <= probability;
}
