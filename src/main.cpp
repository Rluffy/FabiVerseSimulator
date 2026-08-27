#include "headers/WorldHandler.h"
#include "headers/Logger.h"
#include "../src/pugixml/pugixml.hpp"
#include "configs/LogConfig.h"
#include "configs/PersonConfig.h"
#include "configs/TimeConfig.h"
#include "configs/MapConfig.h"

void loadConfigs()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../config/config.xml");

    auto confNode = doc.child("config");

    // Log config
    auto logNode = confNode.child("log");
    LogConfig logConf;
    logConf.file = logNode.child("file").text().as_string();
    logConf.path = logNode.child("path").text().as_string();

    // Person config
    auto personNode = confNode.child("person");
    PersonConfig persConf;
    persConf.startPersonCount = personNode.child("startPersonCount").text().as_int();
    persConf.pregTimeMonths = personNode.child("pregTimeMonths").text().as_int();
    persConf.relLevel = personNode.child("relLevel").text().as_int();
    persConf.adultAgeYears = personNode.child("adultAgeYears").text().as_int();

    // Time config
    auto timeNode = confNode.child("time");
    TimeConfig timeConf;
    std::string timeStr = timeNode.child("startDate").text().as_string();
    int day = std::stoi(timeStr.substr(0, 2));
    int month = std::stoi(timeStr.substr(2, 2));
    int year = std::stoi(timeStr.substr(4, 4));
    timeConf.startDate = {day, month, year};

    // Map config
    auto mapNode = confNode.child("map");
    MapConfig mapConf;
    mapConf.worldLength = logNode.child("worldLength").text().as_int();
    mapConf.worldWidth = logNode.child("worldWidth").text().as_int();
}

int main(int argc, char const *argv[])
{
    World world(0, "World1", {1, 1, 2020}, 100, 100, 36);
    WorldHandler wh(world);
    wh.startSimulation();
    return 0;
}
