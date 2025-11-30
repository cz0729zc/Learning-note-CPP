#include <iostream>
#include <array>

// 演示 C 风格数组（原生数组）和 std::array 的基本用法

void DemoRawArray()
{
    // C 风格数组：大小在编译期固定，不能自动检查越界
    int raw[5] = { 1, 2, 3, 4, 5 };

    std::cout << "[Raw array]" << std::endl;

    // 使用下标访问元素（不会做边界检查）
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "raw[" << i << "] = " << raw[i] << std::endl;
    }

    // 可以通过指针访问
    int* p = raw; // 数组名在大多数表达式中会衰减(decay)为指向首元素的指针
    std::cout << "first element via pointer: " << *p << std::endl;
}

void DemoStdArray()
{
    // std::array（Standard Array）：固定大小数组的封装类模板
    // 模板参数：元素类型 + 大小
    std::array<int, 5> arr1{};                  // 列表初始化为 0
    std::array<int, 5> arr2 = { 1, 2, 3, 4, 5 };

    std::cout << "\n[std::array basic]" << std::endl;

    // fill: 填充整个数组
    arr1.fill(4);

    // at: 带边界检查的访问（运行期检查，下标越界会抛异常）
    std::cout << "arr2.at(4) = " << arr2.at(4) << std::endl;

    // front / back: 第一个 / 最后一个元素
    std::cout << "front = " << arr2.front() << std::endl;
    std::cout << "back  = " << arr2.back()  << std::endl;

    // data: 返回底层原生数组的指针
    std::cout << "data pointer = " << arr2.data() << std::endl;

    // size / empty
    std::cout << "size  = " << arr2.size()  << std::endl;
    std::cout << "empty = " << std::boolalpha << arr2.empty() << std::endl;

    std::cout << "\n[std::array iterate]" << std::endl;

    // 迭代器类型是 std::array<int,5>::iterator，不是 int*
    for (auto it = arr2.begin(); it != arr2.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "\n[std::array compare]" << std::endl;

    // 比较运算符：按元素逐个比较（词典序，lexicographical order）
    std::cout << "arr1 == arr2 : " << (arr1 == arr2) << std::endl;
    std::cout << "arr1 != arr2 : " << (arr1 != arr2) << std::endl;
    std::cout << "arr1 <  arr2 : " << (arr1 <  arr2) << std::endl;
    std::cout << "arr1 <= arr2 : " << (arr1 <= arr2) << std::endl;

    std::cout << "\n[std::array swap]" << std::endl;

    // swap: 交换两个数组（大小和类型必须相同）
    arr1.swap(arr2);

    std::cout << "arr1 after swap: ";
    for (auto v : arr1)
        std::cout << v << ' ';
    std::cout << std::endl;

    std::cout << "arr2 after swap: ";
    for (auto v : arr2)
        std::cout << v << ' ';
    std::cout << std::endl;
}

int main()
{
    DemoRawArray();
    DemoStdArray();

    return 0;
}
                                                                                                                    