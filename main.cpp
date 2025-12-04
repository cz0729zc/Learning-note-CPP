#include <iostream>
#include <string>

// Example类用于演示成员变量的初始化顺序
class Example
{
public:
    Example()
    {
        std::cout << "Creatd Entity" << std::endl;
    }
    Example(int x)
    {
        std::cout << "Created Entity with value: " << x << std::endl;
    }
};

class Entity
{
private:
    std::string m_Name; //第一次默认构造
    int m_Age;    
    Example m_Example; //第一次默认构造 打印 Creatd Entity
public:
    Entity()
        : m_Name("Unknown") , m_Age(0) , m_Example(10) //最好按照初始化列表顺序初始化成员变量,避免一些依赖问题   如果m_Example在此初始化只会构造一次
    {
        // m_Name = "Unknown"; //在函数类初始化会导致m_Name构造两次，效率低下
        m_Example = Example(10); //第二次构造 打印 Created Entity with value: 10
    }

    Entity(const std::string& name)
        : m_Name(name)
    {
        // m_Name = name;
    }

    const std::string& GetName() const
    {
        return m_Name;
    }
};

int main()
{
    Entity e0;
    // std::cout << "Entity e0 name: " <<  e0.GetName() << std::endl;

    // Entity e1("zc");
    // std::cout << "Entity e1 name: " <<  e1.GetName() << std::endl;
    return 0;
}
                                                                                                                    