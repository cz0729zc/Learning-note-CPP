#include <iostream>
#include <string> 

// T 是类型模板参数。调用 Print 时，编译器会根据实参类型推导 T，
// 并生成对应类型的 Print 函数版本。
template<typename T>
void Print(T value)
{
    std::cout << value << std::endl;
}

//当模板参数为int类型时，使用函数模板创建特定版本
template<>
void Print<int>(int value)
{
    std::cout << value << std::endl;
}

//当模板参数为float类型时，使用函数模板创建特定版本
template<>
void Print<float>(float value)
{
    std::cout << value << std::endl;
}

//类模板,
template<typename T,int N>
class Array
{
private:
    T m_Array[N];
public:
    int GetSize() const { return N; }
};

int main()
{
    /*尖括号可以指定模板参数*/
    Print<int>(42);
    Print<int>('a');    

    Print(3.14);
    Print(std::string("Hello, World!"));

    Array<int, 5> array;
    std::cout << "Array size: " << array.GetSize() << std::endl;

    Array<std::string, 10> stringArray;
    std::cout << "String array size: " << stringArray.GetSize() << std::endl;

    return 0;
}