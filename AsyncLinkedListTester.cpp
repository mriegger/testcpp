#include "AsyncLinkedListTester.h"
#include <algorithm>
#include <cassert>

void AsyncLinkedListTester::ThreadFunc(const int id)
{
	for(int i = 0 ; i < NumNumbersToInsert ; ++i)
		m_asyncLinkedList.Insert(id * NumThreads + i);
}

void AsyncLinkedListTester::Run()
{
	SpawnThreads();
	JoinThreads();
	TestResult();
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