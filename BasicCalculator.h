#pragma once

#include <string>
#include <stack>
#include <utility>
#include <cassert>

class BasicCalculator
{
public:

    int Calc(const std::string& s)
    {
        m_iter = 0;
        m_s = s;
        auto x = recurse();
        return x;
    }

    static void Test()
    {
        BasicCalculator c;
        auto result = c.Calc("-(5+4)");
        assert(result == -9);

        result = c.Calc("(5+6)-(9+2)");
        assert(result == 0);

        result = c.Calc("-(5-(9-9-(-5-4))-189)");
        assert(result == 193);
    }

private:

    static int ApplyOp(int a, int b, char op)
    {
        if (op == '+')
        {
            return a + b;
        }
        else if (op == '-')
        {
            return a - b;
        }
        else
        {
            assert(0);
            return INT_MAX;
        }
    }

    static int PopAddAdditionStack(std::stack<int>& s)
    {
        int res = 0;
        while (!s.empty())
        {
            res += s.top();
            s.pop();
        }
        return res;
    }

    int recurse()
    {
        std::stack<int> additionStack;
        int leftNum = 0;
        char op = '+';
        int rightNum = 0;
        while(m_iter < m_s.size())
        {
            if (isdigit(m_s[m_iter]))
            {
                rightNum = rightNum * 10 + m_s[m_iter] - '0';
            }
            else if (m_s[m_iter] == '(')
            {
                m_iter++;
                auto x = recurse();
                leftNum = ApplyOp(leftNum, x, op);
            }
            else if (m_s[m_iter] == '+')
            {
                additionStack.push(ApplyOp(leftNum, rightNum, op));
                leftNum = 0;
                op = '+';
                rightNum = 0;
            }
            else if (m_s[m_iter] == '-')
            {
                additionStack.push(ApplyOp(leftNum, rightNum, op));
                op = '-';
                leftNum = 0;
                rightNum = 0;
            }
            else if (m_s[m_iter] == ')')
            {
                leftNum = ApplyOp(leftNum, rightNum, op);
                return leftNum + PopAddAdditionStack(additionStack);
            }
            m_iter++;
        }

        // reached the end of the string
        leftNum = ApplyOp(leftNum, rightNum, op);
        return leftNum + PopAddAdditionStack(additionStack);
    }

    std::string m_s;
    int m_iter = 0;
};