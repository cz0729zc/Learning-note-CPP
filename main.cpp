#include <iostream>
#include <cstring>

class String
{
private:
    char* m_buffer;
    unsigned int m_size;

public:
    String(const char* string)
    {
        m_size = std::strlen(string);
        m_buffer = new char[m_size + 1];
        std::memcpy(m_buffer, string, m_size + 1);
    }

    // 拷贝构造函数，进行深拷贝
    String(const String& other)
        : m_size(other.m_size)
    {
        m_buffer = new char[m_size + 1];
        std::memcpy(m_buffer, other.m_buffer, m_size + 1);
    }

    // 拷贝赋值运算符，也需要进行深拷贝
    String& operator=(const String& other)
    {
        if (this == &other)
            return *this;

        char* buffer = new char[other.m_size + 1];
        std::memcpy(buffer, other.m_buffer, other.m_size + 1);

        delete[] m_buffer;
        m_buffer = buffer;
        m_size = other.m_size;

        return *this;
    }

    ~String()
    {
        delete[] m_buffer;
    }

    char& operator[](unsigned int index)
    {
        return m_buffer[index];
    }

    const char& operator[](unsigned int index) const
    {
        return m_buffer[index];
    }

    // 友元函数，重载<<运算符
    friend std::ostream& operator<<(std::ostream& stream, const String& string);
};

/*
因为我想直接使用std::cout << str << std::endl，所以我重载了<<运算符，重载后就可以直接使用std::cout << str << std::endl来输出String类的对象了，如果不重载<<运算符，str是做为对象传入std::cout的，这样就会报错，因为std::cout无法直接输出对象，所以我们需要重载<<运算符来告诉编译器，我实际是想输出String类的m_buffer成员变量
*/
std::ostream& operator<<(std::ostream& stream, const String& string)
{
    stream << string.m_buffer;
    return stream;
}

void PrintString(const String& string)
{
    std::cout << string << std::endl;
}

int main()
{
    String str = "Zio zhang";
    String str2 = str; // 这里会调用自定义拷贝构造函数，进行深拷贝

    // 深拷贝后，str2 的 m_buffer 指向另一块内存，所以修改 str2 不会影响 str
    str2[2] = 'X';
    
    PrintString(str);
    PrintString(str2);

    // 如果没有自定义拷贝构造函数，默认浅拷贝会让两个对象指向同一块内存；
    // 作用域结束时两个对象都会 delete[] 同一个地址，容易导致程序崩溃。
}
