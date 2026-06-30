#include <iostream>
#include <string>
#include <vector>

class Vertex
{
public:
    float x, y, z;
};

std::ostream& operator<<(std::ostream& os, const Vertex& v)
{
    os << "Vertex(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

int main()
{
    std::vector<Vertex> vertices;
    std::vector<int> numbers;

    vertices.push_back({0, 1, 2});
    vertices.push_back({3, 4, 5});

    for (const Vertex& v: vertices)
    {
        std::cout << v << std::endl;
    }
        /* 重置数组大小为0 */
    // vertices.clear();

    // for (const Vertex& v: vertices)
    // {
    //     std::cout << v << std::endl;
    // }
        /* 删除第二个元素 */
    vertices.erase(vertices.begin() + 1);

    for (const Vertex& v: vertices)
    {
        std::cout << v << std::endl;
    }
}