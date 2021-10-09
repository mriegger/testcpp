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
        auto x = dfs(s);
        return x;
    }

    static void Test()
    {
        BasicCalculator c;
        int result;
        result = c.Calc("2*3*4");
        assert(result == 24);

        result = c.Calc("-(5+4)");
        assert(result == -9);

        result = c.Calc("(5+6)-(9+2)");
        assert(result == 0);

        result = c.Calc("-(5-(9-9-(-5-4))-189)");
        assert(result == 193);

        result = c.Calc("-(5-(5+6-9*9*2-4-(-5*4))-189)");
        assert(result == 49);
    }

private:

    static int PerformOp(int l, int r, char op)
    {
        switch (op)
        {
        case '+':
            return l + r;
        case '-':
            return l - r;
        case '*':
            return l * r;
        case '/':
            return l / r;
        }
        return INT_MAX;
    }

    int dfs(const std::string& s)
    {
        int sum = 0;
        int lhs = 0;
        int rhs = 0;
        char op = '+';
        while (m_iter < s.size())
        {
            char curr = s[m_iter++];
            if (isdigit(curr))
            {
                rhs *= 10;
                rhs += curr - '0';
            }
            else if (curr == '(')
            {
                rhs = dfs(s);
            }
            else if (curr == ')')
            {
                return sum + PerformOp(lhs, rhs, op);
            }
            else if (curr == '+')
            {
                sum += PerformOp(lhs, rhs, op);
                op = '+';
                lhs = rhs = 0;
            }
            else if (curr == '-')
            {
                sum += PerformOp(lhs, rhs, op);
                op = '-';
                lhs = rhs = 0;
            }
            else if (curr == '/')
            {
                lhs = PerformOp(lhs, rhs, op);
                rhs = 0;
                op = '/';
            }
            else // *
            {
                lhs = PerformOp(lhs, rhs, op);
                rhs = 0;
                op = '*';
            }
        }
        return sum + PerformOp(lhs, rhs, op);
    }

    std::string m_s;
    int m_iter = 0;
};