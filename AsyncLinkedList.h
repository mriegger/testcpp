import Spinlock;

#pragma once

#include <climits>
#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>

class AsyncLinkedList
{
public:

	AsyncLinkedList();
	AsyncLinkedList(const AsyncLinkedList&) = delete;
	AsyncLinkedList& operator=(const AsyncLinkedList&) = delete;
	~AsyncLinkedList();
	void PrintLinkedList();
	std::vector<int> GetLinkedListContents() const;

	void Insert(const int value);
	void Clear();

private:

	struct Node
	{
		std::atomic<Node*> m_next = nullptr;
		int m_value = INT_MAX;
	};

	std::atomic<int> m_atomic = 0;

	std::atomic<Node*> m_head = nullptr;
	std::mutex m_mutex;

	Spinlock m_spinlock;
};