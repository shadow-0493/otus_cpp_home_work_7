#pragma once

#include <vector>
#include <string>

class Printer
{
public:
    virtual ~Printer() = default;
    virtual void print(const std::vector<std::string> &cmds) = 0;
};