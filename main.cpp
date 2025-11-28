#include <iostream>
#include "Log.h"

class Entity
{
public:
    static int x, y;

    static void Print()
    {
        std::cout << x << ", " << y << std::endl;
    }
};

int Entity::x;
int Entity::y;

int main(int, char**)
{
    // Log log;
    // log.SetLevel(Log::LogLevelError);
    // log.warn("This is a warning message.");
    // log.info("This is an info message.");
    // log.error("This is an error message.");

    Entity e;
    // e.x = 5;
    // e.y = 8;
    Entity::x = 5;
    Entity::y = 8;
    Entity::Print();

    Entity e1;
    // e1.x = 10;
    // e1.y = 20;
    Entity::x = 10;
    Entity::y = 20;

    Entity::Print();
    return 0;
}
                                                                                                                    