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

    int recurse()
    {
        int sum = 0;
        int leftNum = 0;
        int currNum = 0;
        char op = '+';
        while (m_iter < m_s.size())
        {         
            const char ch = m_s[m_iter];
            if (ch == ')')
            {
                return sum + leftNum;
            }
            else if (isdigit(ch) || ch == '(')
            {
                if (ch == '(')
                {
                    m_iter++;
                    currNum = recurse();
                }
                else
                {
                    currNum *= 10;
                    currNum += ch - '0';
                }

                // peek at the next character to see if we are done parsing the current number
                if (m_iter + 1 == (int)m_s.size() || !isdigit(m_s[m_iter + 1]))
                {
                    if (op == '*')
                    {
                        leftNum *= currNum;
                    }
                    else if (op == '/')
                    {
                        leftNum /= currNum;
                    }
                    else if (op == '+')
                    {
                        sum += leftNum;
                        leftNum = currNum;
                    }
                    else // op == '-'
                    {
                        sum += leftNum;
                        leftNum = -currNum;
                    }
                    currNum = 0;
                }
            }
            else
            {
                op = ch;
            }
            m_iter++;
        }
        sum += leftNum;
        return sum;
    }

    std::string m_s;
    int m_iter = 0;
};