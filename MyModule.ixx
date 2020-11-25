module; //begins global module fragment

#include <iostream>
#include <string>

export module MyModuleName;

namespace MyModule
{
    export struct S
    {
        int m_integer = 0;
        double m_double = 0.0;
        std::string m_string = "Hello";

        static void PrintName();
    };

    // Needed for use with unordered_set and unordered_map
    inline bool operator==(const S& lhs, const S& rhs)
    {
        return lhs.m_integer == rhs.m_integer;
    }

    // Need for use with map and set
    inline bool operator<(const S& lhs, const S& rhs)
    {
        return lhs.m_double < rhs.m_double;
    }

    // The hash function for S. Make this the second template parameter to the unordered_set
    // i.e. unordered_set<MyModule::S, MyModule::S_Hasher> myUnorderedSet;
    export struct S_Hasher
    {
        size_t operator()(const S& s) const
        {
            return std::hash<double>{}(s.m_double) ^ std::hash<int>{}(s.m_integer);
        }
    };
};