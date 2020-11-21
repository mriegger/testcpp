#include "AsyncLinkedList.h"

AsyncLinkedList::AsyncLinkedList()
{

}

AsyncLinkedList::~AsyncLinkedList()
{
	Clear();
}

void AsyncLinkedList::Insert(const int value)
{
//	std::lock_guard<std::mutex> lck(m_mutex);

	/*
	int expected = 0;
	int desired = 1;
	while (!m_atomic.compare_exchange_strong(expected, desired))
	{
		expected = 0;
	}*/

	Node* newNode = new Node();
	newNode->m_value = value;
	
	while (true)
	{
		auto oldHead = m_head.load();
		newNode->m_next = oldHead;
		bool exchangedOk = m_head.compare_exchange_strong(oldHead, newNode);
		if (exchangedOk)
			break;
	}
	

	//std::atomic<std::shared_ptr<Node>> tn;
	


	//m_atomic.store(0);
}

void AsyncLinkedList::PrintLinkedList()
{
	std::lock_guard<std::mutex> lck(m_mutex);
	const Node* n = m_head;
	while (n)
	{
		std::cout << n->m_value << " ";
		n = n->m_next;
	}
	std::cout << std::endl;
}

std::vector<int> AsyncLinkedList::GetLinkedListContents() const
{
	std::vector<int> res;
	const Node* n = m_head;
	while (n)
	{
		res.push_back(n->m_value);
		n = n->m_next;
	}
	return res;
}

void AsyncLinkedList::Clear()
{
	std::lock_guard<std::mutex> lck(m_mutex);
	std::vector<const Node*> nodes;
	const Node* n = m_head;
	while (n)
	{
		nodes.push_back(n);
		n = n->m_next;
	}
	for (const auto& elem : nodes)
	{
		delete elem;
	}
	m_head = nullptr;
}
