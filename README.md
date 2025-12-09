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

### 核心区别：Pointers vs References

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

## 栈与堆及对象生命周期 (Stack, Heap & Object Lifetime)

这一节结合教程第 44p 和 `main.cpp` 中的示例，梳理几个经常混在一起说的概念：

1. **栈 (Stack)** 上的对象：跟随作用域自动创建和销毁；
2. **堆 (Heap)** 上的对象：用 `new`/`delete` 手动管理生命周期；
3. RAII 思想：让“作用域结束自动销毁”的行为也适用于堆对象——这就是智能指针背后的核心理念。

### 1. 栈上对象：跟随作用域自动销毁

```cpp
class Entity
{
public:
    Entity()
    {
        std::cout << "Created Entity" << std::endl;
    }

    ~Entity()
    {
        std::cout << "Destroyed Entity" << std::endl;
    }
};

int main()
{
    {
        Entity e; // e 在栈上 (stack)，进入作用域时构造，离开作用域时自动析构
    }             // 走到这里，e 离开作用域，自动调用 ~Entity()，打印 "Destroyed Entity"

    return 0;
}
```

> [!IMPORTANT]
> **栈上对象的生命周期**
>
> * 对象作为**局部变量**定义在某个作用域（`{ ... }`）内：
>   * 进入作用域时：调用构造函数，分配栈空间；
>   * 离开作用域时：自动调用析构函数，释放栈空间；
> * 不需要手动 `delete`，编译器会保证“用完就销毁”；
> * 这也是很多 C++ 代码更偏爱“栈对象”的原因：
>   * 生命周期和作用域绑定，简单、直观；
>   * 性能好（栈分配/回收很快）。

### 2. 堆上对象：需要手动 delete 的 new

同一个 `Entity`，如果换成在堆上创建：

```cpp
int main()
{
    {
        // 在堆 (heap) 上分配 Entity
        Entity* e = new Entity(); // 调用构造函数，返回指针

        // ... 使用 e

        delete e; // ❗ 必须手动调用 delete 才会调用 ~Entity()，否则内存泄漏
    }

    return 0;
}
```

> [!TIP]
> **堆对象的两个关键点**
>
> * 用 `new` / `new[]` 创建的对象**不会**在指针变量离开作用域时自动销毁；
> * 它们的生命周期由你决定：
>   * 何时 `delete` / `delete[]`，对象才会真正析构、内存才会被归还；
>   * 如果忘记 `delete`，就会发生**内存泄漏 (Memory Leak)**。

栈和堆的一个常见误区是：

> *“变量是在栈上还是堆上，取决于你是写了 `Entity e;` 还是 `Entity* e = new Entity();`。”*

更准确的说法是：

* `Entity e;`：`e` 本身就是对象，存放在栈上；
* `Entity* e = new Entity();`：
  * 指针变量 `e` 在栈上；
  * 真正的 `Entity` 对象在堆上；
  * 指针变量生命周期结束时，堆上的对象**不会自动销毁**，除非你手动 `delete` 或用 RAII 封装起来。

### 3. 用自定义“智能指针”把堆对象和作用域绑定（RAII）

你在 `main.cpp` 里写了一个非常经典的 RAII 小例子：

```cpp
// 简单的智能指针实现
class ScopedPtr
{
private:
    Entity* m_ptr;
public:
    explicit ScopedPtr(Entity* ptr)
        : m_ptr(ptr)
    {
    }

    ~ScopedPtr()
    {
        delete m_ptr; // 作用域结束时自动释放堆上的 Entity
    }
};

int main()
{
    {
        // 在堆上创建 Entity，把指针交给 ScopedPtr 管理
        ScopedPtr e(new Entity());

        // ... 使用 e 管理的 Entity
    } // 走出作用域时，调用 ScopedPtr::~ScopedPtr()，自动 delete 掉堆上的 Entity

    return 0;
}
```

这里发生了两层“生命周期绑定”：

1. `ScopedPtr e` 是栈对象：
   * 进入花括号作用域时构造；
   * 离开作用域时析构。
2. 在 `ScopedPtr` 的析构函数里 `delete m_ptr;`：
   * 让“`ScopedPtr` 的生命周期结束”这件事，顺带触发“被管理的堆对象销毁”。

这就是 C++ 中非常重要的一个思想：**RAII（Resource Acquisition Is Initialization，资源获取即初始化）**：

> *“把资源（堆内存、文件句柄、锁等）封装进一个对象，让这个对象的构造负责获取资源，析构负责释放资源，从而把资源的生命周期和作用域绑定起来。”*

> [!IMPORTANT]
> **RAII + 栈对象 = 更安全的堆资源管理**
>
> * 不直接在很多地方裸用 `new` / `delete`，而是：
>   * 在构造函数里获取资源（比如 `new Entity()`、`fopen` 等）；
>   * 在析构函数里释放资源（比如 `delete`、`fclose` 等）；
> * 外部代码只需要把这个“管理类”当作一个普通的栈对象使用：
>   * 进入作用域 → 构造 → 获取资源；
>   * 离开作用域 → 析构 → 自动释放资源；
> * 这可以大幅减少“忘记释放资源”的错误，让生命周期变得**可见且可预测**。

### 4. 真正的智能指针：std::unique_ptr / std::shared_ptr

上面的 `ScopedPtr` 是一个“只能管理一个指针、不可拷贝”的简化版智能指针模型。在标准库里，已经为你提供了更完整、更安全的实现：

```cpp
#include <memory>

int main()
{
    {
        // unique_ptr：独占所有权，离开作用域时自动 delete
        std::unique_ptr<Entity> e1 = std::make_unique<Entity>();

        // shared_ptr：共享所有权，最后一个引用被销毁时自动 delete
        std::shared_ptr<Entity> e2 = std::make_shared<Entity>();
    } // 作用域结束，e1 / e2 自己离开栈，内部持有的堆对象也会自动释放

    return 0;
}
```

> [!TIP]
> **现代 C++ 的推荐用法**
>
> * 优先使用栈对象：`Entity e;`，让作用域替你管理生命周期；
> * 如果必须在堆上分配：
>   * 优先使用 `std::unique_ptr` / `std::shared_ptr` 等标准智能指针；
>   * 极少数情况下才直接 `new` / `delete`，而且通常会尽快把原始指针交给某个 RAII 容器管理；
> * Cherno 教程中的 `ScopedPtr` 例子，就是在向你展示“智能指针的核心思想”——
>   实际项目中直接用标准库的智能指针即可，不需要自己重新造轮子。

掌握“栈 vs 堆 vs 生命周期”之后，再回头看构造函数 / 析构函数、RAII、智能指针，会发现它们其实都是在围绕同一个主题：

> **“对象活多久？谁负责在它死的时候把资源收好？”**

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

## const 关键字与只读语义 (const Keyword & Read-Only Semantics)

这一部分结合教程第 34p 和 `main.cpp` 中的代码，总结 C++ 里 `const` 关键字的几个典型用法：

1. 修饰变量：只读变量（有时也叫常量）；
2. 修饰指针：**“指向常量的指针 (pointer to const)”** 和 **“指针常量 (const pointer)”** 以及两者结合；
3. 修饰成员函数：**常量成员函数 (const member function)**；
4. 特殊关键字 `mutable`（Mutable Keyword）：在 `const` 成员函数里可以被修改的成员。

### 1. const 变量：只读但不一定是真正“常量表达式”

```cpp
int main()
{
    const int MAX_AGE = 90;   // 只读变量：之后不能再通过 MAX_AGE 修改这个值

    // MAX_AGE = 100;        // ❌ 错误：不能给 const 变量重新赋值
}
```

> [!IMPORTANT]
> **const 变量的含义**
>
> * 对于变量来说，`const` 表示“**不能通过这个名字修改它**”。
> * 如果初始化时使用的是编译期常量表达式（Constant Expression），有些情况下编译器会把它当成真正的常量用于优化（比如数组大小等）。

### 2. const 与指针：谁是常量，谁是指向的值？

在教程示例和 `main.cpp` 中用的是这三种形式：

```cpp
// 1. 指向常量的指针（pointer to const）
const int* a1 = new int;    // 不能通过 a1 修改指向的值，但可以改变 a1 指向哪儿

// 2. 指针常量（const pointer）
int* const a2 = new int;    // 可以通过 *a2 修改指向的值，但 a2 本身不能指向别处

// 3. 指向常量的指针常量（const pointer to const）
const int* const a3 = new int; // 既不能改指针指向，也不能通过它改指向的值
```

简单记忆：

> [!TIP]
> **看 const 靠近谁**
>
> * `const int*`：`int` 前面有 `const`，表示“指向 **const int** 的指针”，也叫“指向常量的指针 (pointer to const)”，不能改值；
> * `int* const`：`const` 紧挨着 `*`，表示“**指针本身是 const**”，也叫“指针常量 (const pointer)”，不能改指针指向；
> * `const int* const`：两者都 const，指向和指向的值都不能改。

在你的 `main.cpp` 中：

```cpp
const int MAX_AGE = 90;

// 指向常量的指针常量（const pointer to const）
const int* const a = new int;

// *a = 25;            // ❌ 严格来说不合法：a 指向 const int，不应该通过 a 修改值
// a = (int*)&MAX_AGE; // ❌ 不合法：a 是 const 指针，不能改变指向
```

这段代码在严格的 C++ 语义下是有问题的：

* `const int* const a` 表示“不能改 `*a`，也不能改 `a` 本身”；
* 如果编译器没有报错，多半是因为例子还没完全写完，或者是故意留的“反例”。

实践中，你可以用下面的更安全写法来表达“只读视图”：

```cpp
int* raw = new int(25);
const int* view = raw;   // pointer to const：不能通过 view 改值，但 raw 仍然可以改
```

### 3. const 成员函数：承诺“不会修改对象状态”

看 `main.cpp` 中的 `Entity`：

```cpp
class Entity
{
private:
    int* m_X, m_Y;
    mutable int var; // mutable（Mutable Keyword）允许在 const 成员函数中修改
public:
    // 返回一个指向常量的常量指针
    const int* const GetX() const
    {
        // m_X = 5;    // ❌ 错误：不能在 const 成员函数中修改非 mutable 成员
        var = 10;       // ✅ 允许：mutable 成员可以在 const 成员函数中被修改
        return m_X;
    }

    void SetX(int x)
    {
        *m_X = x;
    }
};

void PrintEntity(const Entity& e)
{
    // const Entity& 只能调用 const 成员函数
    std::cout << *(e.GetX()) << std::endl;
}
```

这里有三个层次的 `const`：

1. `const int* const GetX()`：说明返回值类型是“指向 const int 的 const 指针”，调用者不能通过返回值改指向或改值；
2. 函数末尾的 `... GetX() const`：这是 **常量成员函数 (const member function)**，表示：
   * 在函数体内不能修改非 `mutable` 成员；
   * 只能调用同样是 `const` 的其他成员函数；
3. 形参 `const Entity& e`：常量引用（const reference），表示 `PrintEntity` 不会（也不能）修改传进来的 `Entity` 对象。

> [!IMPORTANT]
> **为什么要用 const 成员函数？**
>
> * 从语义上区分“只读操作”和“修改操作”，读代码的人一眼就能看出哪些函数是安全的“观察者 (observer)”；
> * 允许你把对象以 `const Entity&` 的形式传给函数或存入只读容器里，同时仍然可以调用这些“只读接口”；
> * 编译器会帮你检查：你承诺 `const` 的函数里，真的没有去改成员（除了标记为 `mutable` 的）。

### 4. mutable：在 const 成员函数里仍然可变

关键字 `mutable`（Mutable Keyword）用在成员变量上，表示“即使对象是 `const` 的、或者在 `const` 成员函数中，这个成员仍然允许被修改”，典型用法有：

* 缓存字段（比如缓存上一次计算结果）；
* 调试计数器、日志次数等“不影响对象逻辑状态”的统计信息。

在上面的 `Entity` 中：

```cpp
mutable int var;   // 这个成员即使在 const 函数中也可以改

const int* const GetX() const
{
    // m_X = 5;    // ❌ 不允许：m_X 不是 mutable
    var = 10;       // ✅ 允许：var 是 mutable
    return m_X;
}
```

> [!NOTE]
> **mutable 的使用建议**
>
> * `mutable`（Mutable Keyword）是为了那些“不改变对象逻辑状态，但需要在 const 上做点事”的场景准备的；
> * 不要滥用：如果经常需要在 const 上“偷偷修改状态”，通常说明接口设计可以再想想（例如把那些状态抽出去）。

---

### 小结：用 const 表达“承诺”和“约束”

> [!IMPORTANT]
> 从风格和设计角度看，可以把 `const` 理解为一种“写在类型系统里的承诺”：
>
> * `const int` / `const Foo`：承诺“不会通过这个名字去改这块数据”；
> * `const int*` / `Foo* const` / `const int* const`：进一步控制“是不能改指向？还是不能改值？还是都不能改”；
> * `const T&`：告诉调用者“这个函数只读不改，可以放心把自己的对象借给我”；
> * `ReturnType GetXXX() const`：声明“这是一个只读接口”，既方便 `const` 对象使用，也方便编译器帮你检查误修改；
> * `mutable`（Mutable Keyword）：在极少数需要时，允许你在 `const` 环境里修改一些“不算真正状态”的成员。

掌握这些模式之后，`const` 不再只是“不能改的东西”，而是一个帮助你**区分读写、表达设计意图、提升安全性**的小工具。

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

## 成员初始化列表 (Member Initializer List)

这一部分结合教程第 36p 和 `main.cpp` 的示例，总结 C++ 中**成员初始化列表**（Member Initializer List）的用法与好处：

1. 避免“先默认构造，再赋值”的**双重构造**；
2. 某些成员 **必须** 在初始化列表里初始化（如引用、`const` 成员）；
3. 初始化顺序并不是“你写的顺序”，而是按**成员声明顺序**进行。

### 1. 先看一个“构造两次”的例子

你的 `main.cpp` 中有这样一个类：

```cpp
// Example 类用于演示成员变量的初始化顺序
class Example
{
public:
    Example()
    {
        std::cout << "Created Example" << std::endl;
    }

    Example(int x)
    {
        std::cout << "Created Example with value: " << x << std::endl;
    }
};

class Entity
{
private:
    std::string m_Name;  // 第一次：用 std::string 的默认构造
    int m_Age;
    Example m_Example;   // 第一次：调用 Example()，打印 "Created Example"

public:
    Entity()
        : m_Name("Unknown"), m_Age(0), m_Example(10) // 初始化列表
    {
        // m_Name = "Unknown";               // ⚠️ 这里再赋值会触发第二次操作
        m_Example = Example(10);             // 第二次：先默认构造，再拷贝赋值
    }
};
```

关键点：

* 成员对象在进入构造函数体 **之前** 就已经被构造好了：
  * `m_Name` 先通过默认构造构造一次；
  * `m_Example` 先调用 `Example()` 打印一次；
* 在构造函数体内再写 `m_Name = "Unknown";` 或 `m_Example = Example(10);`，只是对已经构造好的对象做一次**赋值**，等于“多做了一遍工作”。

> [!IMPORTANT]
> **为什么要用成员初始化列表？**
>
> * 对于类类型成员（比如 `std::string`、`Example`），
>   * 在初始化列表中写 `m_Example(10)`：**直接调用带参构造函数，一次到位**；
>   * 在构造函数体内再写 `m_Example = Example(10);`：
>     1. 先默认构造一次（无参构造）；
>     2. 再用赋值运算把新值拷过去；
>   * 这就产生了不必要的额外构造/拷贝，效率更差。

### 2. 正确用法：在初始化列表中完成初始化

把上面的 `Entity` 稍微改一下，就可以只构造一次：

```cpp
class Entity
{
private:
    std::string m_Name;
    int m_Age;
    Example m_Example;

public:
    Entity()
        : m_Name("Unknown"),   // 直接调用 std::string(const char*) 构造
          m_Age(0),             // 内置类型也可以在这里初始化
          m_Example(10)         // 直接调用 Example(int) 构造，只构造一次
    {
        // 构造函数体内部可以做一些逻辑，但尽量避免再给成员“重新赋初始值”
    }

    Entity(const std::string& name)
        : m_Name(name),         // 推荐：在初始化列表中就设好名字
          m_Age(0),
          m_Example(0)
    {
        // m_Name = name;      // 不推荐：又一次赋值，多一次开销
    }
};
```

> [!TIP]
> **一个简单经验**
>
> * “谁应该在对象一出生就有确定值”，就尽量放到**初始化列表**里；
> * 构造函数体内部，更适合做流程逻辑、打印日志，而不是“给所有成员再来一遍初始赋值”。

### 3. 有些东西只能在初始化列表里初始化

有几类成员是**不能靠赋值完成初始化**的，它们 **必须** 写在成员初始化列表里：

1. **引用成员**（reference）：引用一旦绑定就不能再指向别的对象；
2. **`const` 成员**：`const` 一旦初始化就不能再赋值；
3. 没有默认构造函数的成员对象：必须显式调用某个构造函数。

示例：

```cpp
class Person
{
private:
    const int m_Id;          // const 成员
    std::string& m_Alias;    // 引用成员

public:
    Person(int id, std::string& alias)
        : m_Id(id),          // ✅ 必须在这里初始化
          m_Alias(alias)     // ✅ 引用也必须在这里绑定
    {
        // m_Id = 10;        // ❌ 错误：const 成员不能在函数体里重新赋值
        // m_Alias = alias;  // ❌ 这里是给引用指向的对象赋值，不是“重新绑定”
    }
};
```

如果不在初始化列表中给它们赋值，编译器就会直接报错，提示你“没有合适的默认构造函数”或“未初始化的 const 成员”等问题。

### 4. 初始化顺序：看声明顺序，不看你写的顺序

一个非常容易踩坑的点：**成员的真正初始化顺序，与初始化列表中写的排列顺序无关，取决于成员在类中声明的先后顺序。**

```cpp
class Foo
{
private:
    Example m_First;
    Example m_Second;

public:
    Foo()
        : m_Second(2),  // 虽然这里写在前面
          m_First(1)    // 但实际初始化顺序仍然是：先 m_First，再 m_Second
    {
    }
};
```

编译器会按照：

1. `m_First`；
2. `m_Second`；

这个声明顺序进行初始化。上面这个写法虽然能编译，但可能会收到编译器警告（warning），提醒你“初始化顺序与声明顺序不一致”。

> [!IMPORTANT]
> **推荐写法**
>
> * 尽量让**初始化列表里的顺序和成员声明顺序一致**：
>
>   ```cpp
>   class Foo
>   {
>   private:
>       Example m_First;
>       Example m_Second;
>
>   public:
>       Foo()
>           : m_First(1),
>             m_Second(2)
>       {}
>   };
>   ```
>
> * 这样一眼就能看出实际的初始化顺序，也能避免因为成员之间有依赖关系时出现隐蔽的 Bug。

---

## 隐式转换与 explicit 关键字 (Implicit Conversion & explicit Keyword)

这一部分结合教程第 41p 和 `main.cpp` 中的 `Entity` 例子，总结 C++ 里的**隐式转换 (Implicit Conversion)** 以及如何用 `explicit` 关键字（explicit Keyword）控制它。

### 1. 一个典型例子：从 `int` / `std::string` 到 `Entity`

`main.cpp` 中的代码：

```cpp
class Entity
{
private:
    std::string m_name;
    int m_Age;

public:
    Entity(const std::string& name)
        : m_name(name), m_Age(-1) {}

    Entity(int age)
        : m_name("Unknown"), m_Age(age) {}
};

void PrintEntity(const Entity& entity)
{
    // 打印逻辑略
}

int main()
{
    PrintEntity(22);                  // 👈 编译器会自动把 22 转成 Entity(22)
    PrintEntity(std::string("Bob")); // 👈 自动调用 Entity(const std::string&)
    PrintEntity(Entity("Alice"));   // 显示构造（显式转换）
}
```

这里有两种**从其它类型到 `Entity` 的转换**：

1. `int → Entity`：通过 `Entity(int age)` 构造函数；
2. `std::string → Entity`：通过 `Entity(const std::string& name)` 构造函数。

> [!IMPORTANT]
> **只带一个参数的构造函数，默认就是“隐式转换构造函数”**
>
> * 当函数参数类型是 `Entity`（或 `const Entity&`），但你传进去的是 `int` / `std::string` 时，
>   编译器会尝试调用这些“单参数构造函数”来**自动构造一个临时 `Entity` 对象**。
> * 这就是所谓的“隐式转换 (Implicit Conversion)”：
>   你没有写 `Entity(22)`，但编译器帮你自动补上了。

这在有些场景下非常方便，但也可能带来“太聪明反而出 bug”的问题。

### 2. 为何要用 explicit：禁止意外的隐式转换

假设我们不希望 `PrintEntity(22);` 这种写法成立，只希望调用者明确地写出“我要构造一个 `Entity`”：

```cpp
class Entity
{
private:
    std::string m_name;
    int m_Age;

public:
    explicit Entity(const std::string& name)
        : m_name(name), m_Age(-1) {}

    explicit Entity(int age)               // 加上 explicit 关键字，防止隐式转换
        : m_name("Unknown"), m_Age(age) {}
};

void PrintEntity(const Entity& entity)
{
}

int main()
{
    // PrintEntity(22);                   // ❌ 编译错误：不再允许从 int 隐式转换为 Entity
    // PrintEntity(std::string("Bob"));   // ❌ 同理，需要显式构造
    PrintEntity(Entity("Alice"));        // ✅ 显式构造，OK

    // 显式调用构造函数
    Entity a = Entity("cherno");
    Entity b = Entity(25);
    Entity c("cherno");
    Entity d(25);
}
```

> [!TIP]
> **explicit（explicit Keyword）的作用**
>
> * 放在构造函数前面，表示“**禁止编译器在需要这个类型的地方自动调用这个构造函数做隐式转换**”；
> * 换句话说：
>   * 允许 `Entity e(10);`、`Entity e = Entity(10);` 这样的**显式构造**；
>   * 禁止 `PrintEntity(10);` 这种**隐式构造 + 传参**。

### 3. 什么时候应该让构造函数支持隐式转换？

并不是所有单参数构造函数都要加 `explicit`，经验上可以这样区分：

1. **“很自然”的转换**，适合保留隐式转换：

   * 例如 `std::string` 有一个 `std::string(const char* s)` 构造函数：

     ```cpp
     void Print(const std::string& s);

     Print("hello");  // 👈 从 const char* 到 std::string 的隐式转换非常自然
     ```

   * 这种“从 C 字符串到 C++ 字符串”的转换几乎没有歧义，保留隐式转换可以大大提高易用性。

2. **有歧义/可能误用的转换**，推荐加上 `explicit`：

   * 比如 `Entity(int age)`：
     * `PrintEntity(22);` 乍一看像是“打印一个整数”；
     * 实际上却是在构造一个 `Entity` 再打印，很容易埋坑；
   * 这种场景更适合强制写成 `PrintEntity(Entity(22));`，代码含义更清晰。

> [!IMPORTANT]
> **经验法则**
>
> * 如果一个构造函数代表的是“类型之间**宽松的、自然的**转换”（如 `const char* → std::string`），可以考虑允许隐式转换；
> * 如果这个转换语义上不够直观，或者将来很可能引起误解/二义性，**一律加上 `explicit`**，强制调用者写出转换意图。

### 4. 除了构造函数，C++ 里还有“转换函数”

更完整的 C++ 还支持**用户自定义转换函数 (User-Defined Conversion Function)**：

```cpp
class Vec2
{
public:
    float x, y;

    // 转换函数：允许 Vec2 隐式转换为 float（长度）
    operator float() const
    {
        return std::sqrt(x * x + y * y);
    }
};

void PrintLength(float len);

void Foo()
{
    Vec2 v{3.0f, 4.0f};
    PrintLength(v); // 👈 编译器会调用 operator float() 把 v 转成长度 5.0f
}
```

这个也可以加 `explicit`（从 C++11 开始），来禁止隐式使用，只允许显式写 `static_cast<float>(v)`。这和在构造函数前加 `explicit` 的思想完全一样：

> [!TIP]
> **统一理解**
>
> * 隐式转换 (Implicit Conversion)：编译器自动帮你在类型之间“搭桥”；
> * `explicit`（explicit Keyword）：告诉编译器“**这座桥必须由我手动走**，你不要偷偷带我过去”。

在当前阶段，你主要先把“单参数构造函数 + explicit”的习惯养好，后面遇到用户自定义转换函数时，会发现两者的设计思想是一致的。

---

## 运算符与运算符重载 (Operators & Operator Overloading)

这一部分结合教程第 42p 和 `main.cpp` 中的 `Vector2` 例子，总结 C++ 中**运算符 (Operator)** 以及**运算符重载 (Operator Overloading)** 的常见用法和设计习惯。

### 1. 先看一个没有运算符重载的写法

假设有一个二维向量类型（例如做 2D 游戏的坐标）：

```cpp
struct Vector2
{
    float x, y;

    Vector2(float x, float y)
        : x(x), y(y) {}

    Vector2 Add(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 Multiply(const Vector2& other) const
    {
        return Vector2(x * other.x, y * other.y);
    }
};

int main()
{
    Vector2 position(4.0f, 4.0f);
    Vector2 speed(0.5f, 1.5f);
    Vector2 powerup(1.1f, 1.1f);

    // 在 Java 中通常只能这样写：通过函数组合
    Vector2 result1 = position.Add(speed.Multiply(powerup));
}
```

这种写法完全没问题，但对于“数学对象”来说，不够直观。如果能写成 `position + speed * powerup` 会更贴近数学公式。

### 2. 给 Vector2 重载运算符

在 C++ 中，可以给自定义类型重载运算符，让它们支持 `+`、`*`、`==` 等操作：

```cpp
struct Vector2
{
    float x, y;

    Vector2(float x, float y)
        : x(x), y(y) {}

    // 普通成员函数版本（便于复用逻辑）
    Vector2 Add(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 Multiply(const Vector2& other) const
    {
        return Vector2(x * other.x, y * other.y);
    }

    // 运算符重载：加法
    Vector2 operator+(const Vector2& other) const
    {
        // 推荐写法：运算符内部调用已有的函数，保持逻辑集中
        return Add(other);
    }

    // 运算符重载：逐元素相乘
    Vector2 operator*(const Vector2& other) const
    {
        return Multiply(other);
    }

    // 运算符重载：相等比较
    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
};

// 重载输出运算符 (Operator<<)，方便直接打印
std::ostream& operator<<(std::ostream& stream, const Vector2& other)
{
    stream << other.x << ", " << other.y;
    return stream;              // 返回 stream 以支持链式调用
}

int main()
{
    Vector2 position(4.0f, 4.0f);
    Vector2 speed(0.5f, 1.5f);
    Vector2 powerup(1.1f, 1.1f);

    // 在 Java 中这是唯一写法：
    Vector2 result1 = position.Add(speed.Multiply(powerup));

    // 在 C++ / C# 中，我们可以用运算符重载，让代码更接近数学公式：
    Vector2 result2 = position + speed * powerup;

    if (result1 == result2)
        std::cout << "The results are equal!" << std::endl;

    if (result1 != result2)
        std::cout << "The result is not a zero vector!" << std::endl;

    std::cout << result2 << std::endl; // 利用 operator<< 打印向量
}
```

> [!IMPORTANT]
> **运算符重载的本质**
>
> * `a + b` 只是 `a.operator+(b)` 或 `operator+(a, b)` 的一种语法糖；
> * `a == b` 对应 `a.operator==(b)`；
> * `std::cout << value` 对应全局函数 `operator<<(std::ostream&, const T&)`；
> * 运算符重载不会“修改语言规则”，只是让你能为自定义类型定义这些“运算符对应的函数”。

### 3. 为什么同时保留函数版本和运算符版本？

在上面的代码里，既有 `Add` / `Multiply` 这样的普通成员函数，又有 `operator+` / `operator*`：

```cpp
Vector2 Add(const Vector2& other) const;
Vector2 Multiply(const Vector2& other) const;

Vector2 operator+(const Vector2& other) const;
Vector2 operator*(const Vector2& other) const;
```

这么做有几个好处：

* **逻辑复用**：运算符内部调用 `Add` / `Multiply`，如果以后改计算逻辑，只需要改一处；
* **可读性选择**：
  * 数学/几何场景下，`a + b * c` 一眼就能看出意思；
  * 某些不适合用运算符表达的场景（例如 `Normalize`、`Rotate`），可以继续用普通成员函数；
* **语言兼容思维**：在不支持运算符重载的语言（例如 Java）里，`Add` / `Multiply` 就是唯一选择，保持这套接口可以帮助你更好迁移思维。

> [!TIP]
> **推荐的写法习惯**
>
> * 定义一个语义清晰的普通函数（例如 `Add` / `Multiply`）；
> * 在对应的 `operator+` / `operator*` 中**直接调用这个函数**；
> * 这样既保留了“函数调用风格”，又享受了“运算符表达式”的简洁。

### 4. 运算符重载的设计建议

虽然 C++ 支持给几乎所有运算符重载，但**并不是应该到处乱用**。一些常见的建议：

> [!IMPORTANT]
> **什么时候适合重载运算符？**
>
> * 类型本身就是“数学对象”或“值语义对象”：如向量 (vector)、矩阵 (matrix)、复数 (complex number)、字符串 (string) 等；
> * 运算符的语义与数学或直觉一致：
>   * `+` 表示“合并 / 相加”；
>   * `-` 表示“相减”；
>   * `*` 对向量可以表示点乘/叉乘/逐元素乘（注意要在文档里说明清楚）；
>   * `==` / `!=` 表示相等性比较；
>   * `<<` 常用于“打印到输出流 (Output Stream)”；
> * 读代码的人看到 `a + b` 时，能大致猜出发生了什么，而不是完全意料之外的副作用。

> [!NOTE]
> **什么时候要谨慎使用运算符重载？**
>
> * 如果运算符做的事情和它在数学上的直觉意思完全不符（例如 `operator+` 里做了文件写入或网络请求）；
> * 如果重载导致表达式难以理解、难以调试（例如重载 `&&` / `||` 隐藏控制流）；
> * 如果只是为了“炫技”，而不是让代码更好读。

总体思路是：

* 运算符重载的目标是**提升可读性和表达力**，而不是制造“魔法”；
* 看看同样逻辑用“普通函数调用方式”和“运算符方式”谁更清楚，如果运算符版本让人一眼就懂，那就是一个好的重载场景。

---

## this 关键字与 this 指针 (this Keyword & this Pointer)

在前面的例子里多次提到：

* 非静态成员函数内部可以直接访问成员变量（好像有一个“隐形的对象”在那儿）；
* 静态成员函数“没有 this 指针”，所以不能访问非静态成员。

这一节结合教程第 43p 和 `main.cpp` 中的 `Entity` 例子，总结一下 C++ 里的 **`this` 关键字 / this 指针 (this pointer)**：

1. `this` 本质上是一个指向“当前对象”的指针，是成员函数的**隐式参数**；
2. 在普通成员函数里，`this` 的类型是 `ClassName*`；在 `const` 成员函数里，是 `const ClassName*`；
3. 可以用 `this->` 来消除成员和形参同名时的歧义，或者在模板/继承场景中显式指出是成员；
4. 常见用法：把当前对象 `*this` 作为引用传给函数，或返回 `*this` 实现链式调用。

### 1. 示例：在构造函数中使用 this

`main.cpp` 中的代码：

```cpp
class Entity;

void PrintEntity(const Entity& e);

class Entity
{
public:
    int x, y;

    Entity(int x, int y)
    {
        // Entity* e = this; // this 是指向当前对象的指针
        // e->x = x;
        // e->y = y;

        this->x = x; // 等价于直接写 x = x，但这里用 this-> 更清晰地表明是“当前对象的 x”
        this->y = y;

        // 在类中调用类外的函数，并以当前对象作为参数传递
        PrintEntity(*this); // *this 是“当前对象本身”，类型是 Entity& 或 const Entity&
    }

    int GetX() const
    {
        const Entity& e = *this; // 在 const 成员函数中，*this 的类型是 const Entity&
        return x;
    }
};

void PrintEntity(const Entity& e)
{
    // 打印或处理 e 的逻辑
}
```

关键点：

* 在 `Entity(int x, int y)` 里，`this` 的真实类型是 `Entity*`；
* `this->x`、`this->y` 只是显式写出了“通过 this 指针访问当前对象成员”的意思，和直接写 `x`、`y` 等价；
* `PrintEntity(*this)` 中的 `*this` 把 `this` 解引用，得到“当前对象本身”作为一个引用，刚好可以绑定到形参 `const Entity& e` 上；
* 在 `GetX() const` 这样的 **const 成员函数 (const member function)** 中，`this` 的真实类型是 `const Entity*`，所以 `*this` 的类型是 `const Entity&`。

> [!IMPORTANT]
> **成员函数签名背后的隐式 this 参数**
>
> * 从编译器的角度看，成员函数
>
>   ```cpp
>   int Entity::GetX() const;
>   ```
>
>   大致等价于一个带额外参数的普通函数：
>
>   ```cpp
>   int GetX(const Entity* const this);
>   ```
>
>   或者写成引用版本也可以理解：
>
>   ```cpp
>   int GetX(const Entity& self);
>   ```
>
> * 也就是说：
>   * 每个非静态成员函数其实都隐式多了一个 `this` 参数；
>   * 非 `const` 成员函数相当于 `Entity* this`，可以修改对象；
>   * `const` 成员函数相当于 `const Entity* this`，承诺不会修改对象（除非成员是 `mutable`）。

### 2. this-> 的常见用途：消除歧义、突出“这是成员”

在构造函数或成员函数中，经常会把形参命名成和成员变量一样：

```cpp
class Point
{
public:
    int x, y;

    Point(int x, int y)
    {
        // 直接写 x = x; // 会把形参 x 赋值给自己，无法访问成员

        this->x = x; // 用 this->x 表示“当前对象的 x 成员”
        this->y = y;
    }
};
```

在这种场景下：

* 裸写 `x` / `y` 指的是“最近的同名变量”（即形参 `x` / `y`）；
* `this->x` / `this->y` 明确表示“当前对象的成员变量”，帮助编译器和读代码的人区分两者。

此外，在一些模板继承的复杂场景里，基类成员需要通过 `this->member` 的方式才能被找到，这里先有个印象即可，后面学模板时会再遇到。

### 3. 把当前对象 *this 传出：参数和链式调用

`this` 最常见的两个“传出”场景是：

1. 把 `*this` 作为引用参数传给函数；
2. 从成员函数中 `return *this;`，实现链式调用。

前者已经在 `PrintEntity(*this);` 中见过，后者可以用一个简单示例来理解：

```cpp
class Vector2
{
public:
    float x, y;

    Vector2(float x, float y)
        : x(x), y(y) {}

    Vector2& Add(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this; // 返回当前对象的引用，支持链式调用
    }
};

int main()
{
    Vector2 v(1.0f, 2.0f);
    Vector2 offset(0.5f, 0.5f);

    v.Add(offset).Add(offset); // 等价于连续两次调用 Add
}
```

> [!TIP]
> **链式调用 (Method Chaining) 的核心：返回 *this**
>
> * 成员函数返回类型用引用（`ClassName&` 或 `const ClassName&`），在函数末尾 `return *this;`；
> * 调用方就可以写成 `obj.Foo().Bar().Baz();`，每一步都在操作“同一个对象”；
> * 这在构建器 (builder)、流式 API（例如 `std::ostream& operator<<`）中非常常见。

结合你在 `main.cpp` 里的实验代码，可以再补充一个对比思路：

```cpp
class Entity
{
public:
    int x, y;

    // 版本 1：修改自己 + 链式调用
    Entity& Add(const Entity& other)
    {
        x += other.x;
        y += other.y;
        return *this;           // 关键：返回 *this 的引用
    }

    // 版本 2：返回新对象，不修改自己
    Entity Add2(const Entity& other) const
    {
        return Entity(x + other.x, y + other.y); // 返回一个“计算结果”的新对象
    }
};

int main()
{
    Entity e1{1, 2};
    Entity e2{3, 4};

    e1.Add(e2).Add(e2);       // 两次 Add 都在修改同一个 e1

    Entity e3 = e2.Add2(e1)   // Add2 不改 e2，本身是 const 成员函数
                    .Add2(e1); // 每一步都返回一个新的临时 Entity
}
```

* `Add`：返回类型是 `Entity&`，`return *this;`，所以所有链式调用都在**同一个对象上累积修改**，类似“在原地堆加”；
* `Add2`：返回类型是按值的 `Entity`，实现也没有改 `this`，而是每次都构造一个“结果对象”，更像**链式计算表达式**，而不是修改原对象。

这两个版本都用到了 `*this`：

* `Add` 中 `return *this;` 表示“把当前对象本身（引用）交给下一步继续用”；
* `Add2` 里虽然没有显式写 `*this`，但可以理解为“把 `this` 指向的对象当作输入参与计算，返回一个全新的结果对象”。

> [!NOTE]
> **设计链式调用时的两个常见模式**
>
> * **命令式 / 原地修改风格**：
>   * 返回类型：`ClassName&`；
>   * 实现：修改成员，然后 `return *this;`；
>   * 效果：`obj.Foo().Bar();` 一路都在改同一个 `obj`，适合“配置对象 / 构建器 / 累积修改”的场景；
> * **函数式 / 返回新值风格**：
>   * 返回类型：按值返回 `ClassName`；
>   * 实现：不改 `this`，只用它的值来构造一个“新对象”返回；
>   * 效果：`auto r = obj.Foo().Bar();`，更像是在“算表达式”，适合数学对象或不可变对象的设计。
>
> 选择哪种风格，取决于你是不是希望“中间步骤”就地修改原对象——但无论哪种方式，`this` 都是那个隐式传进来的“当前对象”，只不过你是把它当成“要被修改的东西”，还是当成“参与计算的输入”。

### 4. this 与静态成员函数

之前在“静态成员与对象实例”一节中提到：

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
```

> [!NOTE]
> **为什么静态成员函数没有 this？**
>
> * 静态成员函数是“挂在类上的函数”，不依赖于某个具体对象，因此编译器不会为它们传入 `this` 参数；
> * 换句话说：
>   * 普通成员函数签名里隐式地有一个 `ClassName* this`；
>   * 静态成员函数则完全没有这个额外参数，只能访问静态成员或通过显式对象参数访问非静态成员。

---

### 小结：把 this 当成“成员函数的第一个参数”

> [!IMPORTANT]
> **统一理解 this 指针 (this pointer)**
>
> * `this` 是一个指向“当前对象”的指针：
>   * 在普通成员函数中，类型大致是 `ClassName* const this`；
>   * 在 `const` 成员函数中，类型大致是 `const ClassName* const this`；
> * 你可以用 `this->member` 显式访问成员，解决与形参/局部变量同名的冲突；
> * 通过 `*this` 可以：
>   * 把当前对象作为引用传给其他函数（如 `PrintEntity(*this)`）；
>   * 从成员函数中 `return *this;`，实现链式调用；
> * 静态成员函数没有 `this`，因此**不能**访问非静态成员，只能访问静态成员或通过显式传入的对象参数来操作对象。

理解 `this` 之后，你可以把“成员函数”在脑中统一想象成“多了一个隐式的第一个参数”的普通函数，这会在后面学习更底层的实现、虚函数表 (vtable)、模板和继承时，帮你更容易看懂编译器在做什么。

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





