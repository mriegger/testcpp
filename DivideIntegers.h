#pragma once

#include <cassert>

// https://leetcode.com/problems/divide-two-integers/solution/
class DivideIntegers
{
public:

    static int Divide(int dividend, int divisor)
    {
        if (dividend < divisor)
            return 0;

        int multiple = 1;
        int testValue = divisor;
        while (true)
        {
            if (testValue == dividend)
            {
                return multiple;
            }
            else if (testValue > dividend)
            {
                const int prevMultiple = multiple >> 1;
                const int prevTestValue = testValue >> 1;
                return prevMultiple + Divide(dividend - prevTestValue, divisor);
            }

            testValue <<= 1;
            multiple <<= 1;
        }
    }

    static void Test()
    {
        assert((16 / 3) == Divide(16, 3));
        assert((93706 / 157) == Divide(93706, 157));
    }
};