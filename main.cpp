#include <iostream>
#include <string>

class Entity;

void PrintEntity(const Entity& e);

class Entity
{
public:
    int x, y;

    Entity(int x, int y)
    {
        // Entity* e = this;
        // e->x = x;
        // e->y = y;
        
        this->x = x;
        this->y = y;

        //在类中调用类外的函数并以类的当前对象作为参数传递，是使用this指针的一个常见场景
        PrintEntity(*this);
    }

    int GetX() const
    {
        const Entity& e = *this;

        return x;
    }

    //链式调用
    Entity& Add(const Entity& other)
    {
        x += other.x;
        y += other.y;
        return *this; // 返回当前对象的引用，支持链式调用
    }

    //失败案例
    //链式调用
    Entity Add2(const Entity other)
    {
        // x += other.x;
        // y += other.y;
        // return *this; // 返回当前对象的引用，支持链式调用
        return Entity(x + other.x, y + other.y);
    }
};

void PrintEntity(const Entity& e)
{

}

int main()
{
    Entity e1(1, 2);
    Entity e2(3, 4);
    e1.Add(e2).Add(e2); // 链式调用示例
    Entity e3 = e2.Add2(e1).Add2(e1); // 链式调用示例

    std::cout << "e1.x: " << e1.x << ", e1.y: " << e1.y << std::endl;

    std::cout << "e3.x: " << e3.x << ", e3.y: " << e3.y << std::endl;
    return 0;
}