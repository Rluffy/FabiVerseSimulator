#pragma once
#include <string>

struct PersonConfig
{
    int startPersonCount;
    int pregTimeMonths;
    int relLevel;
    int adultAgeYears;
    // Probability in % for male, else female
    int maleProbability;
};
