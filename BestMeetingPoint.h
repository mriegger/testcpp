#pragma once

#include <algorithm>
#include <vector>
#include <cassert>
#include <random>

// https://leetcode.com/problems/best-meeting-point/
// Median minimizes the sum of absolute deviations.
class BestMeetingPoint
{
public:

    // fast reference implementation
    static int minTotalDistance2(std::vector<std::vector<int>>& grid) 
    {
        using namespace std;

        int total = 0, X = (int)grid.size(), Y = (int)grid[0].size();
        for (int dim = 0; dim < 2; ++dim) {
            //int c[X * Y]
            std::vector<int> c(X*Y);
            int k = 0;
            for (int x = 0; x < X; ++x)
                for (int y = 0; y < Y; ++y)
                    if (grid[x][y])
                        c[k++] = dim ? y : x;
            nth_element(c.begin(), c.begin() + k / 2, c.begin() + k);
            int louis = c[k / 2];  // (co)median
            while (k--)
                total += abs(louis - c[k]);
        }
        return total;
    }

    static int minTotalDistance(std::vector<std::vector<int>>& grid) 
    {
        using namespace std;

        const int row = (int)grid.size();
        if (0 == row) return 0;
        const int col = (int)grid[0].size();
        int total = 0;
        std::vector<int> posR, posC;
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j) {
                if (grid[i][j] == 1) {
                    posR.emplace_back(i);
                    posC.emplace_back(j);
                }
            }
        int med1 = posR[posR.size() / 2];

        // MKR - so not needed, but good to know that quickselect can be used to find the median
        //nth_element(posR.begin(), posR.begin() + posR.size() / 2, posR.end());
        for (auto pos1 : posR)
            total += abs(pos1 - med1);

        //ranges::sort(posC);
        nth_element(posC.begin(), posC.begin() + posC.size() / 2, posC.end());
        int med2 = posC[posC.size() / 2];
        for (auto pos2 : posC)
            total += abs(pos2 - med2);
        return total;
    }

    static std::vector<std::vector<int>> GenRandomGrid()
    {
        int nr = rand() % 20 + 2;
        int nc = rand() % 20 + 2;
        std::vector<std::vector<int>> res(nr, std::vector<int>(nc, 0));

        for (int i = 0; i < nr; ++i)
        {
            for (int j = 0; j < nc ; ++j)
            {
                int shouldGenerate = rand() % 2;
                if (shouldGenerate == 1)
                {
                    res[i][j] = 1;
                }
            }
        }

        return res;
    }

	static int Test()
	{
        using namespace std;

        for (int i = 0; i < 1000; ++i)
        {
            auto grid = GenRandomGrid();
            auto correct = minTotalDistance2(grid);
            auto test = minTotalDistance(grid);
            assert(test == correct);
        }


        return 0;
	}

private:

};