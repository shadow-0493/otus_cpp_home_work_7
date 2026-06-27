#pragma once

#include "printer.hpp"
#include <vector>
#include <iostream>

class ConsolePrinter : public Printer
{
public:
    void print(std::vector<std::string> &cmds) override
    {
        std::cout << "bulk: ";
        for (size_t i = 0; i < cmds.size(); ++i)
        {
            if (i > 0)
                std::cout << ", ";
            std::cout << cmds[i];
        }
        std::cout << std::endl;
    }
};