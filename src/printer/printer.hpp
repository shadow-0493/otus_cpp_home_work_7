#pragma once

#include <vector>
#include <string>

class Printer
{
public:
    virtual void print(std::vector<std::string> &cmds);
};