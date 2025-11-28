#include <iostream>
#include "Log.h"


int main(int, char**)
{
    Log log;
    log.SetLevel(Log::LogLevelError);
    log.warn("This is a warning message.");
    log.info("This is an info message.");
    log.error("This is an error message.");
    return 0;
}
                                                                                                                    