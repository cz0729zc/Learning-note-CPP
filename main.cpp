#include <iostream>

class Entity
{
public:
    void Print() const
    {
        std::cout << "Entity" << std::endl;
    }
};

class ScopedPtr
{
private:
    Entity* m_Obj;

public:
    ScopedPtr(Entity* entity)
        : m_Obj(entity)
    {

    }

    ScopedPtr(const ScopedPtr&) = delete;
    ScopedPtr& operator=(const ScopedPtr&) = delete;

    ~ScopedPtr()
    {
        delete m_Obj;
    }

    Entity* GetObject()
    {
        return m_Obj;
    }

    const Entity* GetObject() const
    {
        return m_Obj;
    }

    Entity* operator->()
    {
        return m_Obj;
    }

    const Entity* operator->() const
    {
        return m_Obj;
    }
};

int main()
{
    const ScopedPtr entity = new Entity();
    entity->Print();
}
// int main()
// {
//     Entity entity;
//     entity.Print();

//     Entity* ptr = &entity;

//     //这里的箭头实际作用是解引用指针并访问成员函数，等价于(*ptr).Print()，所以两者的效果是一样的。
//     ptr->Print();
//     (*ptr).Print();

//     return 0;
// }
