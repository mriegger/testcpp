import ProducerConsumer;

#include <iostream>
#include <condition_variable>
#include <vector>
#include <mutex>
#include <thread>
#include <cassert>
#include <algorithm>

void ProducerConsumer::Run()
{
	CreateThreads();
	JoinThreads();
}

void ProducerConsumer::ProducerFunc(const int id)
{
	for (size_t i = 0; i < NumDataProducedByEachThread; ++i)
	{
		std::this_thread::yield();
		std::lock_guard<std::mutex> lck(m_mutex);
		m_queue.push_back(id);
	}
}

void ProducerConsumer::ConsumerFunc()
{
	std::vector<int> gatheredResults;
	const size_t targetNumElements = NumDataProducedByEachThread * NumProducerThreads;
	gatheredResults.reserve(targetNumElements);
	while (gatheredResults.size() < targetNumElements)
	{
		std::this_thread::yield();
		std::lock_guard<std::mutex> lck(m_mutex);
		while (m_queue.empty() == false)
		{
			const int value = m_queue.back();
			m_queue.pop_back();
			gatheredResults.push_back(value);
		}
	}

	std::sort(gatheredResults.begin(), gatheredResults.end());
	for (size_t i = 0; i < gatheredResults.size(); ++i)
	{
		assert(gatheredResults[i] == (i / NumDataProducedByEachThread));
	}
}

void ProducerConsumer::JoinThreads()
{
	for (auto& elem : m_producers)
	{
		elem.join();
	}
	m_producers.clear();
	m_consumer.join();
	std::cout << "ProducerConsumer is complete." << std::endl;
}

void ProducerConsumer::CreateThreads()
{
	for (size_t i = 0; i < NumProducerThreads; ++i)
	{
		m_producers.push_back(std::thread(&ProducerConsumer::ProducerFunc, this, i));
	}

	m_consumer = std::thread(std::thread(&ProducerConsumer::ConsumerFunc, this));
}