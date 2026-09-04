#include <string>
#include "Object.h"

using namespace std;

class Food : public Object
{   public:
    Food(string foodname);

    private:
    string foodName;
};