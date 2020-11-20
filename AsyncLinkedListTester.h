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

	static const int NumThreads = 1000;
	static const int NumNumbersToInsert = 1000;

	std::vector<std::thread> m_threads;
	AsyncLinkedList m_asyncLinkedList;
};