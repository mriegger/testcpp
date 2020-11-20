#include "AsyncLinkedList.h"

AsyncLinkedList::AsyncLinkedList()
{

}

AsyncLinkedList::~AsyncLinkedList()
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
}

void AsyncLinkedList::Insert(const int value)
{
	std::lock_guard<std::mutex> lck(m_mutex);
	Node* newNode = new Node();
	newNode->m_value = value;
	newNode->m_next = m_head;
	m_head = newNode;
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