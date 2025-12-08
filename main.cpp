#include <iostream>

class Entity
{
private:
    std::string m_name;
    int m_Age;
public:
    explicit Entity(const std::string& name)
        : m_name(name), m_Age(-1) {}

    explicit Entity(int age)               //在加上explicit关键字后，防止隐式转换
        : m_name("Unknown"), m_Age(age) {}
};

void PrintEntity(const Entity& entity)
{

}

int main() 
{
    // PrintEntity(22);
    // PrintEntity(std::string("cherno"));
    // PrintEntity(Entity("cherno"));

    // 参数调用了类的构造函数实现了隐式转换
    // Entity a = std::string("cherno");
    // Entity b = 25;

    // 显式调用类的构造函数，防止隐式转换
    Entity a = Entity("cherno");
    Entity b = Entity(25);
    Entity c("cherno");
    Entity d(25);


    return 0;
}