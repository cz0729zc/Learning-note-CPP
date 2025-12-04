#include <iostream>
#include <string>

class Entity
{
private:
    int* m_X, m_Y;
    mutable int var; // mutable 允许在const成员函数中修改
public:
    //返回一个指向常量的常量指针,表示指针本身和指针所指向的值都不能被修改
    const int* const GetX() const
    {
        // m_X = 5; // Error: cannot modify member in const method
        var = 10; // Allowed: mutable member can be modified
        return m_X;
    }

    void SetX(int x)
    {
        *m_X = x;
    }
};

void PrintEntity(const Entity& e)
{
    //const Entity 可以使用const成员函数
    std::cout << *(e.GetX()) << std::endl;
}

int main()
{
    const int MAX_AGE = 90;
    
    //常量指针
    // const int* a = new int; 
    //指针常量
    // int* const a = new int;
    //指向常量的指针常量
    const int* const a = new int;

    *a = 25; // Example age
    a = (int*)&MAX_AGE;

    std::cout << "Age: " << *a << std::endl;

    return 0;
}
                                                                                                                    