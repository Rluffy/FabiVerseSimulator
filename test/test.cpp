#include "../src/headers/Logger.h"
#include "../src/headers/PersonHandler.h"
#include "../src/headers/MapHandler.h"
#include "../src/headers/TimeHandler.h"
#include <iostream>

using namespace std;
MapHandler mh(100, 100);
TimeHandler th({1, 1, 2020}, 9);


void loggerTest()
{
    cout << "Logger test started" << endl;
}

void isRelatedTest()
{
    Logger logger("relation.log", th, true);
    PersonHandler ph(mh, th, logger);


    // Prepare persons
    int nextPersonId = 0;

    Person oma(++nextPersonId, {1,1,1934}, "Oma", Gender::Female); //1
    Person opa(++nextPersonId, {1,1,1934}, "Opa", Gender::Male); //2
    Person oma2(++nextPersonId, {1,1,1934}, "Oma2", Gender::Female); // 3
    Person opa2(++nextPersonId, {1,1,1934}, "Opa2", Gender::Male); // 4
    Person mama(++nextPersonId, {1, 1, 2006}, "mama", Gender::Female); // 5
    Person papa(++nextPersonId, {1, 1, 2006}, "papa", Gender::Male); // 6
    Person fabian(++nextPersonId, {1, 1, 2003}, "fabian", Gender::Male); // 7
    Person hannah(++nextPersonId, {1, 1, 2006}, "hannah", Gender::Female); // 8
    Person ingrid(++nextPersonId, {1, 1, 2006}, "ingrid", Gender::Female); // 9

    mama.motherId = oma.id;
    mama.fatherId = opa.id;
    papa.motherId = oma2.id;
    papa.fatherId = opa2.id;

    fabian.motherId = mama.id;
    hannah.motherId = mama.id;
    fabian.fatherId = papa.id;
    hannah.fatherId = papa.id;
    
    
    // check grand parents
  
    vector<Person> persons;
    persons.push_back(fabian);
    persons.push_back(hannah);
    persons.push_back(oma);
    persons.push_back(opa);
    persons.push_back(oma2);
    persons.push_back(opa2);
    persons.push_back(mama);
    persons.push_back(papa);

    ph.persons = persons;
    
    ph.releationLevel = 1;
    cout << fabian.name << " is related with " << hannah.name << (ph.isRelated(fabian,hannah) ? " Passed " : " Not Passed") << endl;
    cout << hannah.name << " is related with " << papa.name << (ph.isRelated(hannah,papa) ? " Passed " : " Not Passed") << endl;
    cout << fabian.name << " is not related with " << ingrid.name << (ph.isRelated(fabian,ingrid) ? " Not Passed " : " Passed") << endl;

   
    cout << fabian.name << " is related with " << oma.name << (ph.isRelated(fabian,oma) ? " Passed " : " Not Passed") << endl;
    cout << fabian.name << " is related with " << opa2.name << (ph.isRelated(fabian,opa2) ? " Passed " : " Not Passed") << endl;
    cout << opa.name << " is not related with " << papa.name << (ph.isRelated(opa,papa) ? " Not Passed " : " Passed") << endl;




}

int main(int argc, char const *argv[])
{
    cout << "Select test" << endl;
    int testCount = 0;

    // Test Options
    cout << " - Log test " << "(" << ++testCount << ")" << endl;
    cout << " - Is Related test " << "(" << ++testCount << ")" << endl;

    int opt;
    cin >> opt;
    switch (opt)
    {
    case 1:
        loggerTest();
        break;
    case 2:
        isRelatedTest();
        break;
    default:
        break;
    }
}
