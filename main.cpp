#include <iostream>
#include <string>
#include <vector>

class Vertex
{
public:
    float x, y, z;

    Vertex(float x, float y, float z) 
        : x(x), y(y), z(z) 
    {

    }

    Vertex(const Vertex& vertex)
        : x(vertex.x), y(vertex.y), z(vertex.z)
    {
        std::cout << "Copied!" << std::endl;
    }
};


int main()
{
    std::vector<Vertex> vertices;
    /* 优化方式一：提前预留空间 */
    // vertices.reserve(3); 
    
    /* 这里出现6次复制的原因有两个：一是空vector第一次插入时会分配容量，之后容量不够还会扩容并复制已有元素；二是Vertex先创建在main函数栈上，传入vector时又会复制一次。*/
    // vertices.push_back(Vertex(1, 2, 3));
    // vertices.push_back(Vertex(4, 5, 6));
    // vertices.push_back(Vertex(7, 8, 9));

    /* 优化方式二：使用emplace_back避免不必要的复制，相当于告诉vector直接在容器内部构造对象，而不是先构造再复制 */
    vertices.emplace_back(1, 2, 3);
    vertices.emplace_back(4, 5, 6);
    vertices.emplace_back(7, 8, 9);
}
