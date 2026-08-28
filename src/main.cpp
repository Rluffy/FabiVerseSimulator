#include "headers/WorldHandler.h"
#include "headers/Logger.h"
#include "../src/pugixml/pugixml.hpp"
#include "configs/Config.h"

Config loadConfigs()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../config/config.xml");

    // not ok status
    if (result.status != 0){
        cout << "Error opening file: " << result.description();
    }

    auto confNode = doc.child("config");

    // World config
    auto worldNode = confNode.child("world");
    WorldConfig worldConf;
    worldConf.simDurationDays = worldNode.child("simDurationDays").text().as_int();

    // Log config
    auto logNode = confNode.child("log");
    LogConfig logConf;
    logConf.file = logNode.child("file").text().as_string();
    logConf.path = logNode.child("path").text().as_string();
    logConf.rewrite = logNode.child("rewrite").text().as_bool();
    logConf.minLogLevel = logNode.child("minLogLevel").text().as_int();

    // Person config
    auto personNode = confNode.child("person");
    PersonConfig persConf;
    persConf.startPersonCount = personNode.child("startPersonCount").text().as_int();
    persConf.pregTimeMonths = personNode.child("pregTimeMonths").text().as_int();
    persConf.relLevel = personNode.child("relLevel").text().as_int();
    persConf.adultAgeYears = personNode.child("adultAgeYears").text().as_int();
    persConf.maleProbability = personNode.child("maleProbability").text().as_int();
    persConf.murderProbability = personNode.child("murderProbability").text().as_int();

    // Time config
    auto timeNode = confNode.child("time");
    TimeConfig timeConf;
    std::string timeStr = timeNode.child("startDate").text().as_string();

    if (timeStr.empty()){
        timeStr="08092003";
        cout << "Empty TimeString";
    }
    int day = std::stoi(timeStr.substr(0, 2));
    int month = std::stoi(timeStr.substr(2, 2));
    int year = std::stoi(timeStr.substr(4, 4));

    timeConf.startDate = {day, month, year};

    // Map config
    auto mapNode = confNode.child("map");
    MapConfig mapConf;
    mapConf.worldLength = mapNode.child("worldLength").text().as_int();
    mapConf.worldWidth = mapNode.child("worldWidth").text().as_int();

    Config conf;
    conf.worldConfig = worldConf;
    conf.logConfig = logConf;
    conf.personConfig = persConf;
    conf.timeConfig = timeConf;
    conf.mapConfig = mapConf;

    return conf;
}
int main(int argc, char const *argv[])
{

   // World world(0, "World1", {1, 1, 2020}, 100, 100, 36);
    Config conf  = loadConfigs();
    WorldHandler wh(conf);
    wh.startSimulation();
    return 0;
}
