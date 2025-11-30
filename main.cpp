#include <iostream>
// #include "Log.h"

class Entity 
{
protected:
    int X, Y;
    void print() {};
public:
    Entity()
    {
        X = 0;
        Y = 0;
        print();
    }
};

class Player : public Entity
{
public:
    Player()
    {
        X = 10;
        Y = 20;
        print();
    }
};


int main()
{
    Entity e;
    e.print();
    return 0;
}
                                                                                                                    