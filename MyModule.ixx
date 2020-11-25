module; //begins global module fragment

#include <iostream>

export module MyModuleName;

namespace MyModule
{
    export struct S
    {
        int m_integer = 0;
        double m_double = 0.0;

        // Needed for unordered_set
        bool operator==(const S&) const = default;

        static void PrintName();
    };

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