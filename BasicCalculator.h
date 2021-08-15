#pragma once

#include <string>
#include <stack>
#include <utility>

class BasicCalculator
{
public:

    static long long ApplyOp(long long a, long long b, char op)
    {
        if (op == '+')
            return a + b;
        else return a - b;
    }

    long long Calc(std::string s)
    {
        m_s = std::move(s);
        auto x = rec(0);
        return x.first;
    }

    std::pair<long long, long long> rec(long long loc)
    {
        long long res = 0;
        char op = '+';
        long long currNum = 0;
        for (long long i = loc; i < m_s.size(); ++i)
        {
            if (isdigit(m_s[i]))
            {
                currNum = currNum * 10 + m_s[i] - '0';
            }
            else if (m_s[i] == '(')
            {
                auto x = rec(i + 1);
                res = ApplyOp(res, x.first, op);
                i = x.second;
            }
            else if (m_s[i] == '+')
            {
                res = ApplyOp(res, currNum, op);
                op = '+';
                currNum = 0;
            }
            else if (m_s[i] == '-')
            {
                res = ApplyOp(res, currNum, op);
                op = '-';
                currNum = 0;
            }
            else if (m_s[i] == ')')
            {
                res = ApplyOp(res, currNum, op);
                return std::make_pair(res, i);
            }
        }
        res = ApplyOp(res, currNum, op);
        return std::make_pair(res, m_s.size());
    }
    std::string m_s;
};