#pragma once

#include <iostream>
#include <string>
#include "Date.h"


class World
{
public:
   // Fix data
   const int id;
   const std::string name;
   // beginning of the world
   const Date startDate;
   const int length;
   const int width;

   const int pregnancyTimeMonths = 9;
   double temperature;

   World(int id, std::string name, Date startDate, int length, int width, double temperature);
};
