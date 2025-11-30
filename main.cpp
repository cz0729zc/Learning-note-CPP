#include <iostream>
// #include "Log.h"

class Entity
{
public:
    std::string GetName() { return "Entity"; }
// virtual std::string GetName() { return "Entity"; }
};

class Player : public Entity
{
private:
    std::string m_Name;
public:
    Player(const std::string& name)
        : m_Name(name)
    {

    }
    std::string GetName() { return m_Name; }
    // std::string GetName() override { return m_Name; }
};

int main()
{
    Entity* e = new Entity();
    std::cout << e->GetName() << std::endl;

    Player* p = new Player("Bob");
    std::cout << p->GetName() << std::endl;

    Entity* e2 = p;
    std::cout << e2->GetName() << std::endl;

    return 0;
}
                                                                                                                    