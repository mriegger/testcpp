#include <atomic>
export module ConcurrencyChecker;

export class ConcurrencyChecker
{
public:

	void SoftLock()
	{

	}

	void SoftUnlock()
	{

	}

private:

	std::atomic<int> m_lockState = 0;
};