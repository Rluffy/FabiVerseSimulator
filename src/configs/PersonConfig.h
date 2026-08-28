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
    // Probability in % for murder, else no murder
    int murderProbability;
};
