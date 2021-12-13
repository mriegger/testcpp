#pragma once

#include <unordered_map>
#include <cassert>

class UnionFind
{
public:

	void Connect(const int a, const int b)
	{
		m_parents[GetParent(a)] = GetParent(b);
	}

	bool IsConnected(const int a, const int b)
	{
		return GetParent(a) == GetParent(b);
	}

	int GetParent(const int a) 
	{
		assert(m_parents.count(a));

		if (m_parents[a] == a)
			return a;
		else
		{
			int parent = GetParent(m_parents[a]);
			// path compression! huge speed increase!
			m_parents[a] = parent;
			return parent;
		}
	}

	void Reset(const int a)
	{
		m_parents[a] = a;
	}

private:

	std::unordered_map<int, int> m_parents;
};