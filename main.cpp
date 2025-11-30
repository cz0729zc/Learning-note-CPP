#include <iostream>
// #include "Log.h"

class Printable
{
public:
    virtual std::string GetClassName() = 0;  
};

class Entity : public Printable
{
public:
    virtual std::string GetName() { return "Entity"; }
    std::string GetClassName() override { return "Entity"; } // 实现纯虚函数，否则 Entity 也是抽象类（不能 new）
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
    // std::string GetName() override { return m_Name; }
    std::string GetClassName() override { return "Player"; } 
};

void Print (Printable* e)
{   
    std::cout << e->GetClassName() << std::endl;
}

int main()
{
    Entity* e = new Entity();
    Print(e);
    Player* p = new Player("Bob");
    Print(p);
    // Entity* e2 = p;
    // Print(e2);
    return 0;
}
                                                                                                                    