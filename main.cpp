#include <iostream>
#include "Log.h"

int main(int, char**){
    LogInit();
    const char* ptr = nullptr;
    if (ptr){  
        Log("Pointer is valid");
        // Log(ptr);
    }else{
        Log("Pointer is null");
    }

}
                                                                                                                    