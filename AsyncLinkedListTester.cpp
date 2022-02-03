#include "AsyncLinkedListTester.h"
#include <algorithm>
#include <cassert>
#include "Timer.h"

void AsyncLinkedListTester::ThreadFunc(const int id)
{
    while (!m_goFlag)
    {

    }

    for (int i = 0; i < NumNumbersToInsert; ++i)
    {
        m_asyncLinkedList.Insert(id * NumNumbersToInsert + i);
    }
}

void AsyncLinkedListTester::Run()
{
    int numIter = 0;
    Timer timer;
    const auto numThreads = std::thread::hardware_concurrency();
    std::cout << "NumThreads: " << numThreads << std::endl;
    while (true)
    {
        m_goFlag = false;
        SpawnThreads();
        timer.Start();
        m_goFlag = true;
        JoinThreads();
        timer.Stop();
        TestResult();
        std::cout << "Completed iteration " << ++numIter << " in " << timer.GetElapsedMilliseconds() << " ms." << std::endl;
        m_asyncLinkedList.Clear();
    }
}

void AsyncLinkedListTester::SpawnThreads()
{
    for (int i = 0; i < NumThreads; ++i)
    {
        m_threads.push_back(std::jthread(&AsyncLinkedListTester::ThreadFunc, this, i));
    }
}

void AsyncLinkedListTester::JoinThreads()
{
    for (auto& elem : m_threads)
    {
        elem.join();
    }
    m_threads.clear();
}

void AsyncLinkedListTester::TestResult()
{
    auto results = m_asyncLinkedList.GetLinkedListContents();
    std::ranges::sort(results);
    assert(results.size() == NumThreads * NumNumbersToInsert);
    for (int i = 0; i < NumThreads * NumNumbersToInsert; ++i)
    {
        assert(i == results[i]);
    }
}