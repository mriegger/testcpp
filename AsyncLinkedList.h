#pragma once

#include <climits>
#include <iostream>
#include <mutex>
#include <vector>

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

private:

	struct Node
	{
		Node* m_next = nullptr;
		int m_value = INT_MAX;
	};

	Node* m_head = nullptr;
	std::mutex m_mutex;
};