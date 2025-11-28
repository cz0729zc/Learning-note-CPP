# CPP 学习随笔

**学习资源**：[Cherno C++ 系列](https://www.bilibili.com/video/BV1VJ411M7WR?spm_id_from=333.788.player.switch&vd_source=c899b188b620426fe3181bd45fd4e21b&p=19)

---

## 指针与引用 (Pointers & References)

引用本质上是给变量取了一个别名。通常情况下，能用引用完成的任务，使用指针也能完成，但引用更安全、更易读。

### 代码示例

```cpp
#include <iostream>
#include "Log.h"

// 常见实参传递：参数为指针
// 创建一个指针变量 value 存储变量 a 的地址
// 在函数内改变值的时候，需要对指针进行解引用 (*value)
void FunctionA(int* value) {
    (*value)++;
}

// 常见实参传递：参数为引用
// 创建一个传递参数变量的引用，此时 value 和 a 等价
// 在函数内改变值的时候正常操作即可，语法更简洁
void FunctionB(int& value) {
    value += 2;
}

int main(int, char**) {
    int a = 5;
    int b = 10;

    int& ref = a;  // 定义 ref 为 a 的引用，此时 ref 和 a 等价
    ref = b;       // 将 b 的值赋给 a (因为 ref 就是 a)，此时 a=10, b=10
    ref++;         // 引用无法重定向 (Reseat)，操作 ref 依旧是在操作 a
                   // 此时 a=11, b=10

    std::cout << "a: " << a << ", b: " << b << std::endl;

    // FunctionA(&a);
    // FunctionB(a);
    
    return 0;
}
```

### 核心区别

   > [!IMPORTANT]
   > **必须初始化**
   > 引用在定义时**必须**初始化（`int& r = a;`），不能像指针那样先声明后赋值。

   > [!TIP]
   > **空值检查**
   > 引用在语义上**不可能为空**，这意味着使用引用通常比指针更安全，因为它保证了对象的有效性。

---

## 类与结构体 (Classes vs Structs)

在 C++ 中，`struct` 和 `class` 的区别非常小，它们在功能上几乎是等价的。

### 代码示例

以下两种定义在功能上完全相同：

```cpp
// Struct 默认成员为 Public
struct PlayerStruct {
    int x, y;
    void Move(int xa, int ya) { x += xa; y += ya; }
};

// Class 默认成员为 Private
class PlayerClass {
public: // 需要显式声明 public 才能达到与 struct 相同的效果
    int x, y;
    void Move(int xa, int ya) { x += xa; y += ya; }
};
```

### 核心区别

1. **默认可见性 (Visibility)**
   * **Class**：成员默认为 `private`。
   * **Struct**：成员默认为 `public`。
   * *注：继承时，`class` 默认为私有继承，`struct` 默认为公有继承。*

2. **使用习惯 (Conventions)**
   * **Struct**：通常用于 **POD (Plain Old Data)** 类型，即只包含数据的被动容器（如向量 `Vec2`、配置参数）。
   * **Class**：通常用于具有复杂功能、封装逻辑和继承体系的主动对象。

> [!NOTE]
> **存在的意义**
> C++ 保留 `struct` 主要是为了保持与 C 语言的向后兼容性。

