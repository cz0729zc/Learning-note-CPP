#include <iostream>
#include "Log.h"

void FunctionA(int* value){
    (*value)++;
}

void FunctionB(int& value){
    value += 2;
}

int main(int, char**){
    int a = 5;
    int b = 10;

    int& ref = a;
    ref = b;
    ref++;
    std::cout << "a: " << a << ", b: " << b << std::endl;

    // FunctionA(&a);
    // std::cout << a << std::endl;
    // FunctionB(a);
    // std::cout << a << std::endl;
    return 0;
}
                                                                                                                    