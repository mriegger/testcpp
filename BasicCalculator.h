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

        result = c.Calc("(((99)))");
        assert(result == 99);

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

    static int UnwindStack(std::stack<int>& s)
    {
        int res = 0;
        while (!s.empty())
        {
            res += s.top();
            s.pop();
        }
        return res;
    }

    int dfs(const std::string& s)
    {
        std::stack<int> additionStack;
        int lastOpSeen = '+';
        int numBeingParsed = 0;

        while (m_iter < s.size())
        {
            const char ch = s[m_iter];
            if (isdigit(ch))
            {
                numBeingParsed *= 10;
                numBeingParsed += ch - '0';
                // check for end of number
                const bool numberEndsHere = m_iter + 1 >= s.size() || !isdigit(s[m_iter + 1]);
                if (numberEndsHere)
                {
                    if (lastOpSeen == '-')
                    {
                        additionStack.push(-numBeingParsed);
                    }
                    else if (lastOpSeen == '+')
                    {
                        additionStack.push(numBeingParsed);
                    }
                    else if (lastOpSeen == '/')
                    {
                        additionStack.top() /= numBeingParsed;
                    }
                    else// if (lastOpSeen == '*')
                    {
                        additionStack.top() *= numBeingParsed;
                    }
                    numBeingParsed = 0;
                }
            }
            else if (ch == '(')
            {
                m_iter++; // move past the (
                int result = dfs(s);
                if (lastOpSeen == '-')
                {
                    additionStack.push(-result);
                }
                else if (lastOpSeen == '+')
                {
                    additionStack.push(result);
                }
                else if (lastOpSeen == '/')
                {
                    additionStack.top() /= result;
                }
                else// if (lastOpSeen == '*')
                {
                    additionStack.top() *= result;
                }
            }
            else if (ch == ')')
            {
                int result = UnwindStack(additionStack);
                return result;
            }
            else // operator
            {
                lastOpSeen = ch;
            }
            m_iter++; // move past the )
        }
        return UnwindStack(additionStack);
    }

    std::string m_s;
    int m_iter = 0;
};