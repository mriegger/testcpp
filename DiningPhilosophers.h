#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <iostream>
#include <semaphore>
#include <chrono>

class DiningPhilosophers
{
public:

	DiningPhilosophers()
	{
	}

	void Run()
	{
		m_semaphore = std::make_unique<ForkSemaphore>(4);
		CreateMutexes();
		CreateThreads();
		while (true)
		{
			std::cout << "Num Plates Eaten: " << m_numPlatesEaten << std::endl;
		}
	}

private:

	void ThreadFunc(int id)
	{
		std::chrono::milliseconds dur(20);
		while (true)
		{
			int leftId = id;
			int rightId = (id + 1) % 5;

			if (id == 4)
			{
			//	std::swap(leftId, rightId);
			}

			m_semaphore->acquire();

		//	m_forkMutexes[leftId].lock();
			std::this_thread::sleep_for(dur); //Leads to deadlock immediately

		//	m_forkMutexes[rightId].lock();
			m_numPlatesEaten++;
			//std::cout << "philo" << id << "is eating" << std::endl;

			std::chrono::milliseconds dur2(rand() % 200 + 100);
			std::this_thread::sleep_for(dur2);

		//	m_forkMutexes[rightId].unlock();
		//	m_forkMutexes[leftId].unlock();
			m_semaphore->release();
		}
	}

	void CreateMutexes()
	{
		std::vector<std::mutex> mutexes(NumPhilosophers);
		m_forkMutexes.swap(mutexes);
	}

	void CreateThreads()
	{
		m_threads.resize(NumPhilosophers);
		for (int i = 0; i < NumPhilosophers; ++i)
		{
			m_threads[i] = std::jthread(&DiningPhilosophers::ThreadFunc, this, i);
		}
	}

	static const int NumPhilosophers = 5;
	uint64_t m_numPlatesEaten = 0;
	std::vector<std::jthread> m_threads;
	std::vector<std::mutex > m_forkMutexes;
	using ForkSemaphore = std::counting_semaphore<4>;
	std::unique_ptr<ForkSemaphore> m_semaphore;
};