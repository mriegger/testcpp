#include <condition_variable>
#include <vector>
#include <mutex>
#include <thread>

export module ProducerConsumer;

export class ProducerConsumer
{
public:

	void Run();

private:

	static const size_t NumProducerThreads = 100;
	static const size_t NumDataProducedByEachThread = 1000;
	static const size_t SizeToFlushList = 10;

	void ProducerFunc(const int id);
	void ConsumerFunc();

	void CreateThreads();
	void SignalWorkerThreadsToStart();
	void JoinThreads();

	std::vector<std::thread> m_producers;
	std::thread m_consumer;
	std::vector<int> m_queue;

	std::mutex m_mutex;
	std::condition_variable m_conditionVariable;

	bool m_workerThreadAllowedToProcess = false;
};