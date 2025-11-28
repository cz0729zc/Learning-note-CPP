#include "Log.h"
#include <iostream>

void Log::SetLevel(LogLevel newLevel)
{
    level = newLevel;
}

void Log::info(const char* message)
{
    if (level <= LogLevelInfo)
        std::cout << "[INFO]: " << message << std::endl;
}

void Log::warn(const char* message)
{
    if (level <= LogLevelWarning)
        std::cout << "[WARNING]: " << message << std::endl;
}

void Log::error(const char* message)
{
    if (level <= LogLevelError)
        std::cout << "[ERROR]: " << message << std::endl;
}