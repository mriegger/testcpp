#pragma once

#include <stack>
#include <cassert>
#include <vector>

// https://leetcode.com/problems/largest-rectangle-in-histogram/
class LargestRectangleInHistogram
{
public:

	static int Calculate(std::vector<int> heights)
	{
		using namespace std;

		vector<int> locationOfSmallerBarToLeft(heights.size(), -1);

		stack<int> indiciesOfSmallerBars;
		// go from left to right and for each element find the 'blocker' the thing smaller than heights[i]
		indiciesOfSmallerBars.push(-1);
		for (int i = 0; i < heights.size(); ++i)
		{
			const int currHeight = heights[i];
			while (indiciesOfSmallerBars.top() != -1 && heights[indiciesOfSmallerBars.top()] >= currHeight)
			{
				// get rid of anything that ISNT a blocker, Anything that isn't bounding the rectangle to the left
				indiciesOfSmallerBars.pop();
			}
			locationOfSmallerBarToLeft[i] = indiciesOfSmallerBars.top();

			// our current height MIGHT be a blocker for future bars
			indiciesOfSmallerBars.push(i);
		}

		int best = 0;
		indiciesOfSmallerBars.push(heights.size());
		for (int i = heights.size() - 1; i >= 0; --i)
		{
			const int currHeight = heights[i];
			// remove any larger bars on the stack, they are NOT bounding us and we want to find out where we can stop the rectangle!!
			while (indiciesOfSmallerBars.top() != heights.size() && heights[indiciesOfSmallerBars.top()] >= currHeight)
			{
				indiciesOfSmallerBars.pop();
			}
			int width = 1 + i - locationOfSmallerBarToLeft[i] - 1 + indiciesOfSmallerBars.top() - i - 1;
			int areaHere = width * currHeight;
			best = max(best, areaHere);
			indiciesOfSmallerBars.push(i);
		}
		return best;
	}

	static void Test()
	{
		int testVal = Calculate({ 2,1,5,6,2,3 });
		assert(testVal == 10);

		testVal = Calculate({ 2,4 });
		assert(testVal == 4);
	}

private:

};