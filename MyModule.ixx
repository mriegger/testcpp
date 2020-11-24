module; //begins global module fragment

#include <iostream>

export module DefaultPrinter;

export struct DefaultPrinter
{
    void PrintHello()
    {
        std::cout << "Hello, I am printing to you from a module" << std::endl;
    }
};