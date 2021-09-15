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
		int right = v.size() - 1;
		int best = v.size();

		while (left <= right)
		{
			const T mid = left + (right - left) / 2;
			const bool comparesGreater = v[mid] < target;
			const bool comparesLess = target < v[mid];

			if (comparesLess && comparesGreater)
			{
				return v.begin() + left;
			}
			else if (comparesGreater) 
			{
				left = mid + 1;
			}
			else // target < mid
			{
				best = mid;
				right = mid - 1;
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
		}
	}

private:

};