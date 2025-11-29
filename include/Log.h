#pragma once

class Log
{
public:
    enum LogLevel
    {
        LogLevelInfo,
        LogLevelWarning,
        LogLevelError
    };

    void SetLevel(LogLevel newLevel);
    void info(const char* message);
    void warn(const char* message);
    void error(const char* message);

private:
    LogLevel level = LogLevelInfo;
};
