#pragma once

#include <vector>
#include <string>

class Printer
{
public:
    virtual ~Printer() = default;
    virtual void print(std::vector<std::string> &cmds) = 0;
};