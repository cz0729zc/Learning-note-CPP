#include <iostream>

class Entity
{
public:
    Entity()
    {
        std::cout << "Created Entity" << std::endl;
    }

    ~Entity()
    {
        std::cout << "Destroyed Entity" << std::endl;
    }
};

// 简单的智能指针实现
class ScopedPtr
{
private:
    Entity* m_ptr;
public:
    ScopedPtr(Entity* ptr)
        :m_ptr(ptr)
    {

    }

    ~ScopedPtr()
    {
        delete m_ptr;
    }
};

int main()
{
    {
        // 在栈上创建实体
        // Entity e;

        // 在堆上创建实体
        // Entity* e = new Entity();

        // 我想在堆上分配内存,并且想在超出作用域时自动释放内存，可以使用智能指针。此时智能指针e是在栈上创建的Entity是在堆上创建的，此时智能指针e超出作用域时会自动调用析构函数，从而释放Entity的内存。
        ScopedPtr e = new Entity();
        // ScopedPtr e(new Entity());
    }

    return 0;
}