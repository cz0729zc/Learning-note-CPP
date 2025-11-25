#include <iostream>

void LogInit(){
    std::cout << "Log Initialized." << std::endl;
}

void Log(const char* message){
    std::cout << "Log: " << message << std::endl;
}