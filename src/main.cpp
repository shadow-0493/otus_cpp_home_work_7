#include <iostream>
#include <string>

#include "parser/command_parser.hpp"

#include "printer/printer.hpp"
#include "printer/file_printer.hpp"
#include "printer/console_printer.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: bulk <N>" << std::endl;
        return 1;
    }

    size_t N = std::stoul(argv[1]);
    CommandParser parser = CommandParser(N);

    ConsolePrinter console_printer;
    FilePrinter file_printer;

    parser.add_printer(&console_printer);
    parser.add_printer(&file_printer);

    std::string line;
    while (std::getline(std::cin, line))
    {
        parser.process(line);
    }
    parser.end_parse();

    return 0;
}