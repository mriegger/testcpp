import ProducerConsumer;

#include <iostream>
#include <condition_variable>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <cassert>
#include <algorithm>

void ProducerConsumer::Run()
{
	auto thread_num = std::thread::hardware_concurrency();
	std::cout << "There are " << thread_num << " threads available on this machine." << std::endl;
	CreateThreads();
	JoinThreads();
}

void ProducerConsumer::ProducerFunc(const int id)
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_conditionVariable.wait(lck, [&]() {return m_workerThreadAllowedToProcess; });
	lck.unlock();

	int numPushed = 0;
	while(numPushed < NumDataProducedByEachThread)
	{
		std::this_thread::yield();
		std::unique_lock<std::mutex> lck(m_mutex);
		m_queue.push_back(id);
		numPushed++;
	}
	m_conditionVariable.notify_one();
}

void ProducerConsumer::ConsumerFunc()
{
	std::vector<int> gatheredResults;
	const size_t targetNumElements = NumDataProducedByEachThread * NumProducerThreads;
	gatheredResults.reserve(targetNumElements);

	while (gatheredResults.size() < targetNumElements)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_conditionVariable.wait(lk, [&] {return m_queue.size() > 0; });
		while (m_queue.empty() == false)
		{
			const int value = m_queue.back();
			m_queue.pop_back();
			gatheredResults.push_back(value);
		}
	}

	std::ranges::sort(gatheredResults);
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
	m_consumer = std::thread(std::thread(&ProducerConsumer::ConsumerFunc, this));
	for (size_t i = 0; i < NumProducerThreads; ++i)
	{
		m_producers.push_back(std::thread(&ProducerConsumer::ProducerFunc, this, i));
	}

	SignalWorkerThreadsToStart();
}

void ProducerConsumer::SignalWorkerThreadsToStart()
{
	std::unique_lock<std::mutex> lck(m_mutex);
	m_workerThreadAllowedToProcess = true;
	m_conditionVariable.notify_all();
}