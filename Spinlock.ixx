#include <atomic>
#include <thread>

export module Spinlock;

export class Spinlock 
{
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:

    void lock() 
    {
        while (flag.test_and_set());
    }

    void unlock() 
    {
        flag.clear();
    }
};
