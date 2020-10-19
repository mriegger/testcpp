#pragma once
#include <vector>

template<typename T>
class ContiguousLinkedList
{
public:

	ContiguousLinkedList(size_t capacity) :m_capacity(capacity)
	{
		m_data.reserve(capacity);
		for (int i = 0 ; i < capacity - 1; ++i)
		{
			Node node;
			node.m_next = i + 1;
			m_data.push_back(node);
		}
		Node lastNode;
		m_data.push_back(lastNode);
	}

	size_t size() const
	{
		return m_used;
	}
	
	size_t capacity() const 
	{
		return m_capacity;
	}

	T& operator[](size_t position)
	{
		assert(position < capacity());
		return m_data[position].m_value;
	}

	const T& operator[](size_t position) const 
	{
		assert(position < capacity());
		return m_data[position].m_value;
	}

	bool HasFreeSpace() const
	{
		return size() < m_capacity;
	}

	int Insert(const T& value)
	{
		if (HasFreeSpace() == false)
			return -1;

		int placeToInsert = m_freeHead;
		if (m_usedHead != -1)
		{
			m_data[m_usedHead].m_prev = placeToInsert;
		}
		m_data[placeToInsert].m_value = value;
		m_freeHead = m_data[placeToInsert].m_next;
		m_data[placeToInsert].m_next = m_usedHead;
		m_usedHead = placeToInsert;
		m_used++;
		return placeToInsert;
	}

	void Remove(int positionToRemove)
	{
		assert(positionToRemove < capacity() && positionToRemove >= 0);
		m_used--;
		if (m_usedHead == positionToRemove)
		{
			m_usedHead = m_data[positionToRemove].m_next;
			if (m_usedHead != -1)
			{
				m_data[m_usedHead].m_prev = -1;
			}
			m_data[positionToRemove].m_next = m_freeHead;
			m_freeHead = positionToRemove;
			return;
		}

		int prev = m_data[positionToRemove].m_prev;
		Node& prevNode = m_data[prev];
		int next = m_data[positionToRemove].m_next;
		if (next != -1)
		{
			Node& nextNode = m_data[next];
			nextNode.m_prev = prev;
		}
		prevNode.m_next = next;

		m_data[positionToRemove].m_next = m_freeHead;
		m_freeHead = positionToRemove;
	}

private:

	struct Node
	{
		T m_value;
		int m_next = -1;
		int m_prev = -1;
	};

	std::vector<Node> m_data;
	int m_freeHead = 0;
	int m_usedHead = -1;
	size_t m_used = 0;
	size_t m_capacity = 0;
};