#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

class TopologicalSort
{
public:
	void AddNode(const char name);
	void AddEdge(const char from, const char to);

	std::string DoSort() const;

	static void Test();

private:

	void Dfs(const char name, std::unordered_set<char>& visited, std::string& result) const;

	std::unordered_map<char, std::set<char>> m_adjacencyList;
	std::vector<std::pair<char, char>> m_edges;
};