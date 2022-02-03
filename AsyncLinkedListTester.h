#pragma once

#include <vector>
#include <thread>
#include <iostream>
#include "AsyncLinkedList.h"

class AsyncLinkedListTester
{
public:

    void Run();

private:

    void SpawnThreads();
    void JoinThreads();
    void ThreadFunc(const int id);
    void TestResult();

    static constexpr int NumThreads = 8;
    static constexpr int NumNumbersToInsert = 100000;

    std::atomic<bool> m_goFlag = false;

    std::vector<std::thread> m_threads;
    AsyncLinkedList m_asyncLinkedList;
};