#include <iostream>
#include <thread>

bool g_stop = false;

void DoWork()
{
    std::cout << "Worker thread started. Thread ID: " << std::this_thread::get_id() << std::endl;

    while (!g_stop)
    {
        std::cout << "Working..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "Worker thread stopping." << std::endl;
}

void finish()
{
    std::cout << "Input thread started. Thread ID: " << std::this_thread::get_id() << std::endl;
    std::cin.get();
    g_stop = true; 
}

int main()
{
    std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

    std::thread worker(DoWork);
    std::thread inputThread(finish);

    // Wait for the worker thread to finish (this will never happen in this example)
    worker.join();
    inputThread.join();

    // std::cout << "All threads finished." << std::endl;
    // std::cin.get();
    return 0;
}