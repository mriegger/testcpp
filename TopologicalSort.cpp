#include "TopologicalSort.h"
#include <algorithm>
#include <cassert>
#include <iostream>

void TopologicalSort::AddNode(const char name)
{
	if (m_adjacencyList.count(name) == 0)
		m_adjacencyList[name].emplace(name);
}

void TopologicalSort::AddEdge(const char from, const char to)
{
	if (m_adjacencyList.count(to) == 0)
	{
		std::set<char> s;
		m_adjacencyList.emplace(std::make_pair(to, s));
	}
	m_adjacencyList[from].emplace(to);
	m_edges.push_back(std::make_pair(from, to));
}

std::string TopologicalSort::DoSort() const
{
	std::string result;
	std::unordered_set<char> visited;
	for (const auto& node : m_adjacencyList)
	{
		const char name = node.first;
		if (!visited.count(name))
		{
			Dfs(name, visited, result);
		}
	}
	std::ranges::reverse(result);
	return result;
}

void TopologicalSort::Dfs(const char name, std::unordered_set<char>& visited, std::string& result) const
{
	assert(m_adjacencyList.count(name) > 0);
	visited.emplace(name);
	const auto iter = m_adjacencyList.find(name);
	for (const char c : iter->second)
	{
		if (!visited.count(c))
			Dfs(c, visited, result);
	}
	result += name;
}

void TopologicalSort::Test()
{
	// https://youtu.be/eL-KzMXSXXI?t=369
	TopologicalSort t;
	t.AddEdge('A', 'D');
	t.AddEdge('E', 'A');
	t.AddEdge('C', 'A');
	t.AddEdge('C', 'B');
	t.AddEdge('D', 'H');
	t.AddEdge('D', 'G');
	t.AddEdge('B', 'D');
	t.AddEdge('C', 'B');
	t.AddEdge('E', 'F');
	t.AddEdge('E', 'D');
	t.AddEdge('F', 'K');
	t.AddEdge('K', 'J');
	t.AddEdge('J', 'M');
	t.AddEdge('J', 'L');
	t.AddEdge('H', 'I');
	t.AddEdge('H', 'J');
	t.AddEdge('G', 'I');
	t.AddEdge('F', 'J');
	t.AddEdge('I', 'L');

	const std::string topo = t.DoSort();

	for (const auto& edge : t.m_edges)
	{
		const char from = edge.first;
		const char to = edge.second;
		const auto posFrom = topo.find(from);
		const auto posTo = topo.find(to);
		assert(posFrom < posTo);
	}
}