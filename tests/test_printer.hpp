#pragma once
#include "../src/printer/printer.hpp"

class TestPrinter : public Printer
{
public:
    void print(std::vector<std::string> &cmds) override
    {
        for (size_t i = 0; i < cmds.size(); ++i)
        {
            if (i > 0)
                output_ += ", ";
            output_ += cmds[i];
        }
        output_ += "\n";
    }

    std::string get_output() const
    {
        return output_;
    }

    void clear_output()
    {
        output_.clear();
    }

private:
    std::string output_;
};