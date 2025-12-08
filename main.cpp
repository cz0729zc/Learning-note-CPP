#include <iostream>

//建议写的时候把Add和运算符重载都写上,方便阅读和使用
struct Vector2
{
    float x, y;

    Vector2(float x, float y)
        : x(x), y(y) {}

    Vector2 Add(const Vector2& other) const
    {
        //函数调用运算符(不常见)
        // return *this + other;
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator+(const Vector2& other) const
    {
        //运算符调用函数(更常见,推荐)
        return Add(other);
        // return Vector2(x + other.x, y + other.y);
    }

    Vector2 Multiply(const Vector2& other) const
    {
        //函数调用运算符(不常见)
        // return *this * other;
        return Vector2(x * other.x, y * other.y);
    }

    Vector2 operator*(const Vector2& other) const
    {
        //运算符调用函数(更常见,推荐)
        return Multiply(other);
        // return Vector2(x * other.x, y * other.y);
    }

    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
};

//重载输出运算符,这里的stream参数是由cout传入的
std::ostream& operator<<(std::ostream& stream, const Vector2& other)
{
    stream << other.x << ", " << other.y;
    return stream;
}

int main() 
{
    Vector2 position(4.0f, 4.0f);
    Vector2 speed(0.5f, 1.5f);
    Vector2 powerup(1.1f, 1.1f);

    //在JAVA中这是唯一的选择，但在C++中我们可以使用运算符重载来让代码更简洁
    Vector2 result1 = position.Add(speed.Multiply(powerup));
    Vector2 result2 = position + speed * powerup;

    //用运算符代替函数调用更简洁,C++和C#都推荐这么做但JAVA不支持运算符重载
    // if(result1.equals(result2))
    //     std::cout << "The results are equal!" << std::endl;

    if(result1 == result2)
        std::cout << "The results are equal!" << std::endl;

    if(result1 != result2)
        std::cout << "The result is not a zero vector!" << std::endl;
    

    std::cout << result2 << std::endl;


    return 0;
}