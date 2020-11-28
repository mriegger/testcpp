#include "AsyncLinkedListTester.h"
#include <algorithm>
#include <cassert>

void AsyncLinkedListTester::ThreadFunc(const int id)
{
    for (int i = 0; i < NumNumbersToInsert; ++i)
    {
        m_asyncLinkedList.Insert(id * NumNumbersToInsert + i);
    }
}

void AsyncLinkedListTester::Run()
{
    int numIter = 0;
    while (true)
    {
        SpawnThreads();
        JoinThreads();
        TestResult();
        std::cout << "Completed " << ++numIter << std::endl;
        m_asyncLinkedList.Clear();
    }
}

void AsyncLinkedListTester::SpawnThreads()
{
    for (int i = 0; i < NumThreads; ++i)
    {
        m_threads.push_back(std::thread(&AsyncLinkedListTester::ThreadFunc, this, i));
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
    std::sort(results.begin(), results.end());
    assert(results.size() == NumThreads * NumNumbersToInsert);
    for (int i = 0; i < NumThreads * NumNumbersToInsert; ++i)
    {
        assert(i == results[i]);
    }
}