#include <iostream>
#include <string>
#include <array>
#include <vector>

void Print()
{
    std::cout << "Hello, World!" << std::endl;
}

void PrintNumber(int X)
{
    std::cout << "Number: " << X << std::endl;
}

void PrintValue(int X)
{
    std::cout << "Vlaue : " << X << std::endl;
}

void ForEach(std::vector<int> numbers, void(*Function)(int))
{
    for (int number : numbers)
    {
        Function(number);
    }
}

int main()
{
    // Print();
    void(*message)() = Print;
    // message = Print;
    message();

    auto message2 = Print;
    message2();

    void(*messageNumber)(int) = PrintNumber;
    messageNumber(5);

    auto messageNumber2 = PrintNumber;
    messageNumber2(10);

    typedef void(*HelloworldFunction)();

    HelloworldFunction hellowrold = Print;
    hellowrold();

    typedef void(*PrintNumberFunction)(int);
    PrintNumberFunction printNumber = PrintNumber;
    printNumber(100);

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    ForEach(numbers, PrintValue);
    ForEach(numbers, [](int X){ std::cout << "Lambda Value: " << X << std::endl;});


    return 0;
}