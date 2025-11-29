#include <iostream>
// #include "Log.h"

class Entity
{
public:
    float X, Y;

    Entity()
    {
        X = 0.0f;
        Y = 0.0f;
        std::cout << "Create Entity" << std::endl;
    }

    Entity(float x, float y)
    {
        X = x;
        Y = y;
    }

    ~Entity()
    {
        std::cout << "Destroy Entity" << std::endl;
    }

    void Print()
    {
        std::cout << "X: " << X << ", Y: " << Y << std::endl;
    }
};

void Function()
{
    Entity e;
    e.Print();
}

int main(int, char**)
{
    // Entity e;
    // e.Print(); // Uninitialized values

    // Entity e2(5.0f, 10.0f);
    // e2.Print(); // Initialized values

    Function();

    return 0;
}
                                                                                                                    