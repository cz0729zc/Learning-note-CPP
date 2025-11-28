# CPP 学习随笔

**学习资源**：[Cherno C++ 系列](https://www.bilibili.com/video/BV1VJ411M7WR?spm_id_from=333.788.player.switch&vd_source=c899b188b620426fe3181bd45fd4e21b&p=19)

---

## 指针与引用 (Pointers & References)

引用本质上是给变量取了一个别名。通常情况下，能用引用完成的任务，使用指针也能完成，但引用更安全、更易读。

### 代码示例：类级 static

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

---

## 类作用域与枚举访问 (Log::LogLevelWarning)

在学习 `Log` 日志类时，有一个容易混淆的问题：为什么在 `main.cpp` 中要写 `Log::LogLevelWarning`，而不能写成 `log.LogLevelWarning`？

### 思考过程小结：

```cpp
class Log {
public:
    enum LogLevel {
        LogLevelInfo,
        LogLevelWarning,
        LogLevelError
    };

    void SetLevel(LogLevel level);
};

int main() {
    Log log;
    log.SetLevel(Log::LogLevelWarning); // ✅ 正确用法
    // log.LogLevelWarning;             // ❌ C++ 不支持这种写法
}
```

* `LogLevel` 是定义在 `class Log` 内部的**枚举类型**，`LogLevelWarning` 是这个枚举里的**枚举值**。
* 它们属于 **类的作用域**，而不是某个对象的成员变量，所以在类外访问时需要写成 `Log::LogLevel`、`Log::LogLevelWarning`。
* 对象 `log` 上只有具体这一个对象的**成员变量值和成员函数**，例如 `log.SetLevel(...)`，并不会“挂着”一个叫 `LogLevelWarning` 的字段，所以 `log.LogLevelWarning` 是不合法的。

可以类比：

* `std::cout` 中的 `std::` 是命名空间作用域；
* `Log::LogLevelWarning` 中的 `Log::` 是类作用域；
* 而 `log` 只是一个对象实例，用来调用成员函数（`log.SetLevel(...)`），而不是用来访问枚举定义。

---

## 静态成员与对象实例 (Static Members)

在学习 `Entity` 时，遇到两个典型问题：

1. 类中成员变量/函数加上 `static` 会发生什么？
2. 为什么只在类里写 `static int x, y;` 会出现 `undefined reference to Entity::x`？

### 代码示例

```cpp
class Entity
{
public:
    static int x, y;      // 静态成员变量：属于类本身，而不是某个对象

    static void Print()   // 静态成员函数：不需要 this 指针
    {
        std::cout << x << ", " << y << std::endl;
    }
};

// 类外必须提供一次定义，否则会链接错误（undefined reference）
int Entity::x;
int Entity::y;

int main()
{
    Entity e;
    Entity::x = 5;
    Entity::y = 8;
    Entity::Print(); // 输出 5, 8

    Entity e1;
    Entity::x = 10;
    Entity::y = 20;
    Entity::Print(); // 输出 10, 20（e 和 e1 看到的是同一份 x, y）
}
```

### 思考过程小结

* `static int x, y;` 在类内只是**声明**，不会分配内存；真正的定义需要在类外写 `int Entity::x; int Entity::y;`。
* 静态成员变量是“类级别”的，全局共享一份，所有对象（`e`、`e1`）看到的是同一个 `x`、`y`。
* 静态成员函数没有 `this` 指针，只能访问静态成员（如 `x`、`y`），不能直接访问非静态成员；如果把 `x, y` 改成非静态，而 `Print` 还保持 `static`，会编译报错。
* 如果希望每个对象有自己的坐标，而不是共享一份，就应该去掉 `static`，把 `Print` 也改成普通成员函数，然后通过对象访问：`e.x = 5; e.y = 8; e.Print();`。



