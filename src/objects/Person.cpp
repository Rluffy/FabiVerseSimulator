#include "../headers/Person.h"
#include <cstdlib>

Person::Person(int id, Date birthdate, string name, Gender gender, Coordinate coordinate)
    : id(id),
      birthdate(birthdate),
      name(name),
      gender(gender)

{
    srand(time(0));
    this->coordinate = coordinate;
}

Coordinate Person::move(int worldLength, int worldWidth)
{
    int min = -1;
    int max = 1;

    int xCordinate = coordinate.x;
    int yCordinate = coordinate.y;
    Coordinate newCoordinate;

    do
    {

        int moveDirectionX = rand() % (max - min + 1) + min;
        xCordinate += movementPerHour * moveDirectionX;
        if (xCordinate > worldLength || xCordinate < 0)
        {
            xCordinate -= movementPerHour * moveDirectionX;
        }

        int moveDirectionY = rand() % (max - min + 1) + min;
        yCordinate += movementPerHour * moveDirectionY;
        if (yCordinate > worldWidth || yCordinate < 0)
        {
            yCordinate -= movementPerHour * moveDirectionY;
        }

        newCoordinate = {xCordinate, yCordinate};
    } while (newCoordinate == coordinate);

    return newCoordinate;
}