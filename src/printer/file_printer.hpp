#include "printer.hpp"
#include <chrono>
#include <sstream>
#include <vector>
#include <fstream>

class FilePrinter : public Printer
{
public:
    void print(std::vector<std::string> &cmds) override
    {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(now).count();
        std::ostringstream name;
        name << "bulk" << sec << ".log";

        std::ofstream file(name.str());
        for (size_t i = 0; i < cmds.size(); ++i)
        {
            if (i > 0)
                file << ", ";
            file << cmds[i];
        }
        file << std::endl;
    }
};