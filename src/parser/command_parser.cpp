#include "command_parser.hpp"
#include <stdexcept>

CommandParser::CommandParser(size_t cmd_block_size) : cmd_block_size_(cmd_block_size)
{
    buffer_.reserve(cmd_block_size_);
}

void CommandParser::add_printer(Printer *printer)
{
    printers_.push_back(printer);
}

void CommandParser::_print()
{
    if (buffer_.empty())
        return;

    for (auto *printer : printers_)
    {
        printer->print(buffer_);
    }

    buffer_.clear();
}

void CommandParser::process(const std::string &line)
{
    if (line == "{")
        nest_level_++;

    if (line == "}")
        nest_level_--;

    if (nest_level_ < 0)
    {
        throw std::runtime_error("Error: unexpected '}' without opening '{'");
    }

    switch (state_)
    {
    case CommandParserState::BUILD_STATIC_BLOCK:

        if (nest_level_ != 0)
        {
            _print();
            cmd_count_ = 0;
            state_ = CommandParserState::BUILD_DYNAMIC_BLOCK;
        }
        else
        {
            buffer_.push_back(line);
            ++cmd_count_;

            if (cmd_count_ == cmd_block_size_)
            {
                _print();
                cmd_count_ = 0;
            }
        }
        break;

    case CommandParserState::BUILD_DYNAMIC_BLOCK:

        if ((line != "{") && (line != "}"))
            buffer_.push_back(line);

        if (nest_level_ == 0)
        {
            _print();
            state_ = CommandParserState::BUILD_STATIC_BLOCK;
        }

        break;

    default:
        throw std::runtime_error("Unknown state of the parser");
    }
}

void CommandParser::end_parse()
{
    if (nest_level_ > 0)
    {
        buffer_.clear();
        cmd_count_ = 0;
    }
    else
    {
        _print();
    }
}