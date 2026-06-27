#include "..\printer\printer.hpp"

enum class CommandParserState
{
    BUILD_STATIC_BLOCK,
    BUILD_DYNAMIC_BLOCK
};

class CommandParser
{
public:
    explicit CommandParser(size_t cmd_block_size);
    void add_printer(Printer *printer);
    void process(const std::string &line);
    void end_parse();

private:
    size_t cmd_block_size_;
    CommandParserState state_ = CommandParserState::BUILD_STATIC_BLOCK;
    int nest_level_ = 0;
    size_t cmd_count_ = 0;
    std::vector<std::string> buffer_;
    std::vector<Printer *> printers_;

    void _print();
};