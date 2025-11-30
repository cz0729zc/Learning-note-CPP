# CPP 学习随笔

**学习资源**：[Cherno C++ 系列](https://www.bilibili.com/video/BV1VJ411M7WR?spm_id_from=333.788.player.switch&vd_source=c899b188b620426fe3181bd45fd4e21b&p=1)

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

### 核心区别：class vs struct

   > [!IMPORTANT]
   > **必须初始化**
   > 引用在定义时**必须**初始化（`int& r = a;`），不能像指针那样先声明后赋值。

   > [!TIP]
   > **空值检查**
   > 引用在语义上**不可能为空**，这意味着使用引用通常比指针更安全，因为它保证了对象的有效性。

---

## 类与结构体 (Classes vs Structs)

在 C++ 中，`struct` 和 `class` 的区别非常小，它们在功能上几乎是等价的。

### 代码示例：构造与析构

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

## 数组基础 (Arrays Basics)

这一部分结合教程第 31p 和 `main.cpp` 的示例，总结 C++ 中两种常见的“定长数组”形式：

1. **原生数组**（也叫 C 风格数组，C-style array）；
2. **`std::array`（Standard Array）**：标准库提供的定长数组封装类模板（Class Template）。

### 1. 原生数组（C 风格数组）

```cpp
#include <iostream>

void DemoRawArray()
{
    // 在栈上创建一个大小为 5 的 int 数组，大小在编译期固定
    int raw[5] = { 1, 2, 3, 4, 5 };

    std::cout << "[Raw array]" << std::endl;

    // 使用下标访问元素（不会做边界检查）
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "raw[" << i << "] = " << raw[i] << std::endl;
    }

    // 数组名在大多数表达式中会“衰减”(decay) 为指向首元素的指针
    int* p = raw;
    std::cout << "first element via pointer: " << *p << std::endl;
}
```

> [!IMPORTANT]
> **原生数组的几个特点**
>
> * 大小在编译期固定，不能在运行时动态改变大小（`int raw[n]` 中的 `n` 必须是编译器能确定的常量，标准 C++ 下如此）。
> * 名字在大多数表达式中会衰减(decay) 为 `T*` 指针，这也是很多 C 风格函数参数写 `int*` 的原因。
> * 使用 `raw[i]` 不会做越界检查，`i` 写错了编译器也不会提示，运行时可能导致未定义行为(Undefined Behavior)。

原生数组适合简单、底层、对性能和内存布局有较强要求的场景，但安全性不高，需要自己非常小心边界管理。

### 2. `std::array`：更安全的定长数组封装

`std::array`（Standard Array）是 C++ 标准库提供的一个模板类，用来封装“**固定大小、在栈上分配**”的数组：

```cpp
#include <iostream>
#include <array>

void DemoStdArray()
{
    // 模板参数：元素类型 + 大小
    std::array<int, 5> arr1{};                  // 列表初始化为 0
    std::array<int, 5> arr2 = { 1, 2, 3, 4, 5 };

    std::cout << "[std::array basic]" << std::endl;

    // fill: 填充整个数组
    arr1.fill(4);

    // at: 带边界检查的访问（运行期检查，下标越界会抛异常）
    std::cout << "arr2.at(4) = " << arr2.at(4) << std::endl;

    // front / back: 第一个 / 最后一个元素
    std::cout << "front = " << arr2.front() << std::endl;
    std::cout << "back  = " << arr2.back()  << std::endl;

    // data: 返回底层原生数组的指针，可以与 C 风格 API 交互
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
```

> [!TIP]
> **相对于原生数组，`std::array` 提供的额外能力**
>
> * 成员函数：`size()` / `empty()` / `fill()` / `front()` / `back()` / `data()` 等；
> * 安全访问：`at(index)` 会做运行期边界检查，越界时抛出异常而不是静默地产生未定义行为；
> * 迭代器(Iterator)：可以用 `begin()` / `end()` 配合 `for` 循环和标准算法(`std::sort` / `std::find` 等)；
> * 支持比较运算符：按元素做词典序(lexicographical order) 比较，适合排序或放在关联容器里。

### 3. 什么时候用原生数组，什么时候用 `std::array`？

> [!IMPORTANT]
> **简单经验**
>
> * 需要固定大小的栈数组，并且这个数组会在多个函数之间传递、操作：优先用 `std::array`；
> * 只是在局部函数里暂时用一下，而且非常简单（比如 `int a[3] = {1,2,3};`）：用原生数组也没问题；
> * 需要和 C 接口交互时：可以用 `std::array` + `.data()` 拿到底层指针，两边都能兼顾。

从学习角度，原生数组可以帮助你理解指针、内存布局这些底层概念，而在真实项目中，`std::array`（和后面会学到的 `std::vector`，Standard Vector）会更常用，更安全也更好组合到泛型算法里。

---

## 类作用域与枚举访问 (Log::LogLevelWarning)

在学习 `Log` 日志类时，有一个容易混淆的问题：为什么在 `main.cpp` 中要写 `Log::LogLevelWarning`，而不能写成 `log.LogLevelWarning`？

### 思考过程小结：枚举与类作用域

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

这一部分结合 Cherno 的几集讲解和自己的代码实践，总结 C++ 中 `static` 的几个核心场景：

1. **类的静态成员变量 / 静态成员函数**：属于“类本身”，而不是某个具体对象。
2. **静态成员 vs 普通成员**：是否每个对象一份，还是所有对象共享一份。
3. **全局变量 vs static 成员**：为什么更推荐把“全局状态”放到类中。

### 1. 类的静态成员变量与链接错误

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

// 类外必须提供一次“定义”，否则会链接错误（undefined reference）
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

> [!IMPORTANT]
> **声明 vs 定义**
>
> * 类内的 `static int x, y;` 只是**声明**，不会分配内存。
> * 必须在类外写一次 `int Entity::x; int Entity::y;` 作为**定义**，为静态成员分配存储空间。

### 2. static 成员变量 vs 普通成员变量

```cpp
// 每个对象一份（非 static）
class EntityInstance {
public:
    int x, y;

    void Print() {
        std::cout << x << ", " << y << std::endl;
    }
};

// 全部对象共享一份（static）
class EntityShared {
public:
    static int x, y;

    static void Print() {
        std::cout << x << ", " << y << std::endl;
    }
};

int EntityShared::x;
int EntityShared::y;

int main() {
    EntityInstance a;
    a.x = 1; a.y = 2;

    EntityInstance b;
    b.x = 3; b.y = 4;

    a.Print(); // 1, 2
    b.Print(); // 3, 4  -> 各自独立

    EntityShared::x = 5;
    EntityShared::y = 6;
    EntityShared::Print(); // 5, 6

    EntityShared::x = 7;
    EntityShared::y = 8;
    EntityShared::Print(); // 7, 8  -> 所有“实体”看到的都是同一份值
}
```

> [!IMPORTANT]
> **对象 vs 类本身**
>
> * 普通成员变量：每个对象一份，`a.x` 和 `b.x` 是两块不同的内存。
> * 静态成员变量：类共享一份，通过 `EntityShared::x` 访问，所有对象看到的是同一块内存。

### 3. static 成员函数 与 普通成员函数

```cpp
class Example {
public:
    int value = 0;          // 普通成员变量
    static int s_Value;     // 静态成员变量

    void NormalMethod() {
        // 可以访问 this 上的一切
        value = 10;
        s_Value = 20;
    }

    static void StaticMethod() {
        // 没有 this 指针，只能访问静态成员
        // value = 10;      // ❌ 编译错误：不能在 static 里访问非 static 成员
        s_Value = 20;       // ✅ OK
    }
};

int Example::s_Value = 0;
```

> [!TIP]
> **搭配使用的规则**
>
> * 想访问对象自己的状态：用「非 static 成员变量 + 非 static 成员函数」。
> * 想做“全类共享的状态 / 工具函数”：用「static 成员变量 + static 成员函数」。

### 4. 全局变量 vs static 成员：为什么更推荐 static

```cpp
// 全局变量写法（不推荐）
int g_LogLevel = 0;

void Log(const char* message) {
    if (g_LogLevel > 0)
        std::cout << message << std::endl;
}

// 使用 static 成员的写法（更清晰）
class Logger {
public:
    static int s_LogLevel;

    static void Log(const char* message) {
        if (s_LogLevel > 0)
            std::cout << message << std::endl;
    }
};

int Logger::s_LogLevel = 0;
```

> [!NOTE]
> **为什么不用裸的全局变量**
>
> * 全局变量任何地方都能改，作用域太大，不利于维护。
> * static 成员变量挂在类名下（例如 `Logger::s_LogLevel`），表达了“这个状态属于 Logger”，更符合面向对象的设计。

### 5. 小结：什么时候用 static？

> [!NOTE]
> **static 使用场景小抄**
>
> * 想让“所有对象共享某个状态”（例如：计数器、全局配置）：用 `static` 成员变量。
> * 想写一个不依赖具体对象的工具函数，但和这个类逻辑相关：用 `static` 成员函数。
> * 想要“每个对象有自己的那一份”：不要用 `static`，用普通成员变量 + 普通成员函数。
> * 避免随手丢一个真正的全局变量，优先考虑放到类的 `static` 成员里，让状态有一个清晰的“归属”。

---

## 构造函数与析构函数 (Constructors & Destructors)

在 C++ 中，构造函数和析构函数是一对“对象生命周期钩子”：

* **构造函数 (Constructor)**：对象被创建时自动调用，用来初始化成员、申请资源。
* **析构函数 (Destructor)**：对象生命周期结束、被销毁时自动调用，用来清理和释放资源。

### 代码示例：继承与对象大小

```cpp
class Entity
{
public:
    float X, Y;

    // 默认构造函数：不带参数
    Entity()
    {
        X = 0.0f;
        Y = 0.0f;
        std::cout << "Create Entity" << std::endl;
    }

    // 带参数构造函数：构造时传入初始值
    Entity(float x, float y)
    {
        X = x;
        Y = y;
    }

    // 析构函数：对象离开作用域/被销毁时自动调用
    ~Entity()
    {
        std::cout << "Destroy Entity" << std::endl;
    }

    void Print()
    {
        std::cout << "X: " << X << ", Y: " << Y << std::endl;
    }
};

void Function()
{
    Entity e;   // 这里调用 Entity() 构造函数
    e.Print();
}              // 函数结束，e 离开作用域，调用 ~Entity() 析构函数

int main()
{
    // Entity e;            // 调用默认构造函数，X/Y 被初始化为 0
    // e.Print();

    // Entity e2(5.0f, 10.0f); // 调用带参数构造函数
    // e2.Print();             // 打印初始化后的值

    Function();
    return 0;
}
```

### 核心要点

> [!IMPORTANT]
> **构造函数**
>
> * 名字与类名相同，没有返回类型（连 `void` 都不能写）。
> * 在对象创建时自动调用，只会在“开始生命周期”时调用一次。
> * 常用于：初始化成员变量、申请资源（内存、文件句柄等）。
> * 可以重载多个构造函数，根据参数不同选择不同的构造方式。

> [!IMPORTANT]
> **析构函数**
>
> * 名字为 `~ClassName()`，同样没有返回类型、没有参数。
> * 在对象生命周期结束时自动调用：
>   * 栈对象离开作用域（如函数结束）。
>   * `delete` 堆对象。
> * 常用于：成对地释放构造函数中申请的资源（RAII 思想：资源获取即初始化）。
> * 一个类只能有一个析构函数，不能重载。

> [!TIP]
> **和普通函数的区别**
>
> * 构造/析构函数由编译器在恰当的时机自动调用，不需要（也不能）手动像普通函数那样随便调用。
> * 普通成员函数可以随时被调用多次，而构造函数只在“创建时”调用一次，析构函数只在“销毁时”调用一次。

---

## 类的继承 (Inheritance)

这一部分用 `Entity` / `Player` 的例子，理解几件事：

1. `class Player : public Entity` 里的 `public` 是在控制**继承后的可见性**，不是“只继承 public 成员”。
2. 派生类对象内存布局中，**始终包含一个完整的基类子对象**。
3. 成员的对齐（alignment）和补齐（padding）会影响 `sizeof` 的结果。

### 代码示例

```cpp
class Entity
{
private:
    int ID;      // 私有成员，仍然占据对象内存，但派生类代码不能直接访问

public:
    float X, Y;

    void Move(float xa, float ya)
    {
        X += xa;
        Y += ya;
    }
};

class Player : public Entity   // public 继承：Player 是一种 Entity
{
public:
    const char* Name;          // 新增成员：名字指针

    void PrintName()
    {
        std::cout << Name << std::endl;
    }
};

int main()
{
    std::cout << sizeof(Entity) << std::endl;
    std::cout << sizeof(Player) << std::endl;
}
```

在 64 位环境下（`int` 4 字节，`float` 4 字节，指针 8 字节），可能看到类似输出：

```text
12
24
```

### 思考过程小结：虚函数与绑定方式

> [!IMPORTANT]
> **public 继承的含义**
>
> * `public` 并不是“只继承 public 成员”，而是控制“继承后 public/protected 成员在派生类中的可见性”。
> * `class Player : public Entity`：
>   * `Entity` 的 `public` 成员，在 `Player` 中仍然是 `public`；
>   * `Entity` 的 `protected` 成员，在 `Player` 中仍然是 `protected`；
>   * `Entity` 的 `private` 成员仍然存在于对象内存里，但只能通过 `Entity` 自己（或友元）访问，`Player` 的成员函数不能直接访问。
> * `public` 继承表达的是“is-a” 关系：`Player` 是一种 `Entity`，可以在需要 `Entity` 的地方使用 `Player`。

> [!IMPORTANT]
> **对象内存布局与对齐**
>
> * 派生类对象中，包含一个完整的基类子对象，再加上派生类自己的成员。
> * `Entity`：`int ID` (4) + `float X` (4) + `float Y` (4) → 逻辑大小 12 字节。
> * `Player`：
>   * 先放一个 `Entity` 子对象（12 字节，会为了后续成员对齐补齐到 16 `8的倍数`）。
>   * 再放一个 8 字节的指针 `Name`。
>   * 所以总大小通常是 16 + 8 = 24，而不是“12 + 8 = 20”。
> * `sizeof` 统计的是“数据成员 + 对齐填充”，不包含成员函数代码、static 成员等。

> [!TIP]
> **如何直观理解 sizeof 的变化**
>
> * 先按顺序写出所有成员的大小，再考虑对齐要求（通常是最大成员类型的对齐，比如指针 8 字节）。
> * 基类作为子对象嵌入派生类时，也要按照派生类的对齐需求进行补齐，这会让“只多了 4 字节”在派生类中表现为多出 8 甚至更多。
> * 可以用 `static_assert(sizeof(...))` 或打印 `sizeof` 的方式，配合图示帮助自己建立“对象布局”的直觉。

---

## 虚函数与多态 (Virtual Functions & Polymorphism)

这一部分用 `Entity` / `Player` 的例子理解：

1. 非虚函数调用是**静态绑定**：看的是指针/引用的「类型」。
2. 虚函数调用是**动态绑定**：看的是指针/引用实际指向对象的「真实类型」。
3. 多态的典型写法是：用基类指针/引用指向派生类对象，通过虚函数实现“同一接口，不同行为”。

在此基础上，引入**纯虚函数 (pure virtual)** 和 **抽象类 (abstract class)** 的概念，可以定义一组“必须由子类实现的接口”。

### 代码示例：非虚函数 vs 虚函数

```cpp
#include <iostream>
// #include "Log.h"

class Entity
{
public:
    // std::string GetName() { return "Entity"; }
    virtual std::string GetName() { return "Entity"; }
};

class Player : public Entity
{
private:
    std::string m_Name;
public:
    Player(const std::string& name)
        : m_Name(name)
    {

    }
    // std::string GetName() { return m_Name; }
    std::string GetName() override { return m_Name; }
};

int main()
{
    Entity* e = new Entity();
    std::cout << e->GetName() << std::endl;

    Player* p = new Player("Bob");
    std::cout << p->GetName() << std::endl;

    Entity* e2 = p;
    std::cout << e2->GetName() << std::endl;

    return 0;
}                
```

### 思考过程小结

> [!IMPORTANT]
> **静态绑定 vs 动态绑定**
>
> * 非虚函数调用（没有 `virtual`）：
>   * 编译期就根据“指针/引用的类型”决定调用哪个版本，称为**静态绑定**。
>   * `Entity* e = &p; e->GetName();` 会调用 `Entity::GetName`，即使 `e` 实际指向的是 `Player` 对象。
> * 虚函数调用（带 `virtual`）：
>   * 运行时根据“指针/引用实际指向的对象类型”来决定调用哪个版本，称为**动态绑定**或多态。
>   * `Entity2* e = &p; e->GetName();` 会调用 `Player2::GetName`，因为底层真实对象是 `Player2`。

> [!TIP]
> **何时使用虚函数**
>
> * 有一个统一的“基类接口”（如 `GetName()` / `Update()` / `Draw()`），而具体行为由不同子类实现时，就应该在基类上声明为 `virtual`。
> * 通过基类指针/引用（如 `Entity*` 或 `Entity&`）操作一组不同子类对象时，虚函数可以让调用端“不关心具体类型”，只关心接口。
> * 多态基类通常还要提供一个 `virtual` 析构函数，保证通过基类指针 `delete` 派生类对象时析构链完整执行。

### 纯虚函数与抽象类

```cpp
#include <iostream>
// #include "Log.h"

class Printable
{
public:
    virtual std::string GetClassName() = 0;  
};

class Entity : public Printable
{
public:
    virtual std::string GetName() { return "Entity"; }
    std::string GetClassName() override { return "Entity"; } // 实现纯虚函数，否则 Entity 也是抽象类（不能 new）
};

class Player : public Entity
{
private:
    std::string m_Name;
public:
    Player(const std::string& name)
        : m_Name(name)
    {

    }
    // std::string GetName() override { return m_Name; }
    std::string GetClassName() override { return "Player"; } 
};

void Print (Printable* e)
{   
    std::cout << e->GetClassName() << std::endl;
}

int main()
{
    Entity* e = new Entity();
    Print(e);
    Player* p = new Player("Bob");
    Print(p);
    // Entity* e2 = p;
    // Print(e2);
    return 0;
}
```

> [!IMPORTANT]
> **纯虚函数和抽象类的关系**
>
> * 含有至少一个纯虚函数（`= 0`）的类称为“抽象类 (abstract class)，不能直接实例化：`Printable p;` / `new Printable()` 都是非法的。
> * 抽象类通常用作“接口类”：只定义函数签名（接口），具体实现交给子类完成。
> * 如果派生类（如 `Entity`）没有实现所有继承来的纯虚函数，那么它本身也会是抽象类，同样无法被实例化。

> [!TIP]
> **多态 + 接口的常见组合**
>
> * 用一个抽象基类（如 `Printable`）声明一组纯虚函数，定义“这个家族的通用能力”。
> * 各个子类（`Entity` / `Player` / 其他）实现这些纯虚函数，提供各自的行为。
> * 对外只暴露一个接受基类指针/引用的接口（如 `void Print(Printable* p)`），调用端完全不需要关心传进来的是哪个具体子类，只关心可以调用哪些虚函数。


---

## 访问控制与封装 (Access Control & Encapsulation)

访问控制关键字 `public` / `private` / `protected` 不会改变“程序是否能跑起来”（只要你不去写非法代码），但会极大影响：

* 对外暴露的 **API 边界** 是什么；
* 以后能否在**不改调用方代码**的前提下重构实现；
* 类内部细节是否被“随便依赖”，导致维护成本越来越高。

### 基本语义回顾

```cpp
class Entity
{
public:      // 对所有地方可见（类内、类外、子类）
    void Move(float xa, float ya);

protected:   // 只对本类和子类可见，对类外隐藏
    int X, Y;
    void PrintPosition();

private:     // 只对本类可见，连子类都不能直接访问
    int m_ID;
};

class Player : public Entity
{
public:
    Player()
    {
        X = 10;              // ✅ 可以访问基类的 protected 成员
        Y = 20;
        PrintPosition();     // ✅ 调用基类的 protected 成员函数
        // m_ID = 1;        // ❌ 不允许访问基类的 private 成员
    }
};

int main()
{
    Entity e;
    // e.X = 5;             // ❌ X 是 protected，类外不可访问
    // e.PrintPosition();   // ❌ 同上
    e.Move(1.0f, 2.0f);     // ✅ 通过 public 接口操作对象
}
```

> [!IMPORTANT]
> **三种访问级别的直观记忆**
>
> * `public`：**给所有人用的**，是这个类对外承诺的稳定接口。
> * `protected`：**给自己和子类用的**，是继承体系内部共享的实现细节。
> * `private`：**只给自己用的**，完全内部实现，任何外部代码都不应该直接依赖。

### 什么时候用 public？

> [!TIP]
> 可以把 `public` 理解成“这个类对外公开的 API 文档”：
>
> * 构造函数、析构函数（如果需要对外创建/销毁）。
> * 业务动作：例如 `Move()`、`Attack()`、`Update()`、`Draw()`。
> * 对外查询状态的只读函数：例如 `GetX()`、`GetName()`、`IsAlive()`。
>
> 一旦某个成员被标记为 `public`，调用方就可以在任何地方使用它，你之后要修改这个接口就必须考虑所有调用点的影响。

实践经验：

* **默认不要急着 public**，先假设所有东西都是内部实现；
* 当你发现“外部确实需要调用这段逻辑”时，再把它升级为 `public` 接口；
* `public` 区域保持尽量小而干净，让调用方一眼就能看出“这个类是干什么的、我能怎么用它”。

### 什么时候用 private？

> [!IMPORTANT]
> 只要是“实现细节”而不是“对外承诺”，就应该优先设为 `private`：
>
> * 内部使用的辅助函数，例如 `RecalculateBounds()`、`ReloadConfig()`；
> * 不希望被外部随便改的内部状态，例如 `m_ID`、缓存字段、临时标志位；
> * 任何你“以后可能会改掉/删掉/重构”的东西，都适合藏在 `private` 里。

这样做的好处是：

* 外部代码**没法直接依赖你的内部实现**，未来你可以放心地重写实现，只要维持 `public` 接口不变即可；
* 如果发现某个功能“外部确实需要”，可以随时把对应成员从 `private` 提升为 `public` 或加一个 `public` 包装函数。

### 什么时候用 protected？

`protected` 主要用于有继承关系时，在“完全封死的 private”和“完全开放的 public”之间提供一个折中：

* 让基类和派生类之间可以共享一些内部实现细节；
* 但仍然不对普通调用方暴露这些实现。

典型例子就是上面的 `X, Y` 和 `PrintPosition()`：

* 作为调用方，只需要一个 `Move()` 接口，不需要知道坐标是怎么存储/打印的；
* 作为派生类（`Player`、`Enemy`），可能需要在某些情况下直接读写坐标或打印调试信息，这时访问 `protected` 成员会比走一堆 `public` 包装函数更方便。

> [!NOTE]
> 使用 `protected` 时要克制：
>
> * 如果你确定这个信息只会在基类内部用，优先 `private`；
> * 只有当“确实需要在多个派生类里重复用到这段内部逻辑”时，才考虑把它提到 `protected`；
> * 一味把东西都设为 `protected`，会让整个继承层级变成“半公开”的状态，子类之间互相踩来踩去，反而难以维护。

### 小结：访问控制更多是“设计问题”

从编译器角度看，`public` / `private` / `protected` 只是访问权限检查规则，不会改变对象在内存中的布局，也不会改变“语法正确的代码”运行出来的结果。

但从工程实践角度看：

> [!IMPORTANT]
> **访问控制本质上是“帮你划清边界”的工具**：
>
> * `public` 定义类的“对外承诺”和长期稳定的接口；
> * `protected` 为继承层级提供一个共享实现的“内部通道”；
> * `private` 把实现细节锁在类内部，防止被随意依赖。

这也是很多教程（包括 Cherno）强调的：

* 不要纠结“访问控制会不会影响性能/能不能跑”，
* 更重要的是用它来表达你的**设计意图**，让读你代码的人一眼就能看出：
  * 这个类真正提供给外部用的是什么；
  * 哪些只是内部实现细节，随时可能会变。





