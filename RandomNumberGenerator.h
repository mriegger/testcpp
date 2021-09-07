#pragma once

#include <random>

class RandomNumberGenerator
{
public:
	float GetRandomFloat(const float minVal, const float maxVal) 
	{
		std::uniform_real_distribution<float> uniform_distance(minVal, maxVal);
		return uniform_distance(m_generator);
	}

	double GetRandomDouble(const double minVal, const double maxVal)
	{
		std::uniform_real_distribution<double> uniform_distance(minVal, maxVal);
		return uniform_distance(m_generator);
	}

	int GetRandomInt(const int minVal, const int maxVal)
	{
		std::uniform_int_distribution<int> uniform_distance(minVal, maxVal);
		return uniform_distance(m_generator);
	}

private:
	std::default_random_engine m_generator;
};