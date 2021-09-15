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

    int recurse()
    {
        int res = 0;
        char op = '+';
        int currNum = 0;
        while(m_iter < m_s.size())
        {
            if (isdigit(m_s[m_iter]))
            {
                currNum = currNum * 10 + m_s[m_iter] - '0';
            }
            else if (m_s[m_iter] == '(')
            {
                m_iter++;
                auto x = recurse();
                res = ApplyOp(res, x, op);
            }
            else if (m_s[m_iter] == '+')
            {
                res = ApplyOp(res, currNum, op);
                op = '+';
                currNum = 0;
            }
            else if (m_s[m_iter] == '-')
            {
                res = ApplyOp(res, currNum, op);
                op = '-';
                currNum = 0;
            }
            else if (m_s[m_iter] == ')')
            {
                res = ApplyOp(res, currNum, op);
                return res;
            }
            m_iter++;
        }
        res = ApplyOp(res, currNum, op);
        return res;
    }

    std::string m_s;
    int m_iter = 0;
};