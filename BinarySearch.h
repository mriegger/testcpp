#pragma once

#include <vector>
#include <cassert>

class BinarySearch
{
public:

	template<typename T>
	inline static std::vector<T>::const_iterator LowerBound(const std::vector<T>& v, const T target)
	{
		int left = 0;
		int right = (int)v.size() - 1;
		int best = (int)v.size();

		while (left <= right)
		{
			const T mid = left + (right - left) / 2;
			const bool targetGreaterMid = v[mid] < target;
			if (targetGreaterMid)
			{
				left = mid + 1;
			}
			else 
			{
				best = mid;
				right = mid - 1;
			}
		}
		return v.begin() + best;
	}

	template<typename T>
	inline static std::vector<T>::const_iterator UpperBound(const std::vector<T>& v, const T target)
	{
		int left = 0;
		int right = (int)v.size() - 1;
		int best = (int)v.size();

		while (left <= right)
		{
			const T mid = left + (right - left) / 2;
			const bool targetLessMid = target < v[mid];
			if (targetLessMid)
			{
				best = mid;
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}
		return v.begin() + best;
	}

	static void Test()
	{
		const std::vector<int> v = { 1,4,6,8 };
		for (int i = -10; i <= 10; ++i)
		{
			assert(lower_bound(v.begin(), v.end(), i) == LowerBound(v, i));
			assert(upper_bound(v.begin(), v.end(), i) == UpperBound(v, i));
		}
	}

private:

};