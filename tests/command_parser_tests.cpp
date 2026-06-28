#include <gtest/gtest.h>
#include "../src/parser/command_parser.hpp"
#include "test_printer.hpp"
#include <sstream>

class CommandParserTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        parser.add_printer(&printer);
    }

    void TearDown() override
    {
        printer.clear_output();
    }

    void processLines(const std::string &input)
    {
        std::istringstream stream(input);
        std::string line;
        while (std::getline(stream, line))
        {
            parser.process(line);
        }
    }

    TestPrinter printer;
    CommandParser parser{3};
};

TEST(CommandParserConstructorTest, ZeroBlockSize)
{
    EXPECT_THROW(CommandParser(0), std::invalid_argument);
}

TEST(CommandParserConstructorTest, NegativeBlockSize)
{
    EXPECT_THROW(CommandParser(-1), std::invalid_argument);
    EXPECT_THROW(CommandParser(-5), std::invalid_argument);
    EXPECT_THROW(CommandParser(-100), std::invalid_argument);
}

TEST(CommandParserConstructorTest, PositiveBlockSize)
{
    EXPECT_NO_THROW(CommandParser(3));
}

TEST_F(CommandParserTest, StaticBlock_ExactSize)
{
    processLines("cmd1\ncmd2\ncmd3\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1, cmd2, cmd3\n");
}

TEST_F(CommandParserTest, StaticBlock_LessThanSize)
{
    processLines("cmd1\ncmd2\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1, cmd2\n");
}

TEST_F(CommandParserTest, StaticBlock_MoreThanSize)
{
    processLines("cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1, cmd2, cmd3\ncmd4, cmd5\n");
}

TEST_F(CommandParserTest, StaticBlock_MultipleExact)
{
    processLines("cmd1\ncmd2\ncmd3\ncmd4\ncmd5\ncmd6\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1, cmd2, cmd3\ncmd4, cmd5, cmd6\n");
}

TEST_F(CommandParserTest, StaticBlock_SingleCommand)
{
    processLines("cmd1\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\n");
}

TEST_F(CommandParserTest, DynamicBlock_Basic)
{
    processLines("cmd1\n{\ncmd2\ncmd3\ncmd4\n}\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\ncmd2, cmd3, cmd4\n");
}

TEST_F(CommandParserTest, DynamicBlock_StartsImmediately)
{
    processLines("{\ncmd1\ncmd2\ncmd3\n}\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1, cmd2, cmd3\n");
}

TEST_F(CommandParserTest, DynamicBlock_Empty)
{
    processLines("cmd1\n{\n}\ncmd2\ncmd3\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\ncmd2, cmd3\n");
}

TEST_F(CommandParserTest, DynamicBlock_WithStaticAfter)
{
    processLines("cmd1\n{\ncmd2\ncmd3\n}\ncmd4\ncmd5\ncmd6\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\ncmd2, cmd3\ncmd4, cmd5, cmd6\n");
}

TEST_F(CommandParserTest, NestedDynamicBlock_SingleLevel)
{
    processLines("cmd1\n{\ncmd2\n{\ncmd3\n}\ncmd4\n}\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\ncmd2, cmd3, cmd4\n");
}

TEST_F(CommandParserTest, NestedDynamicBlock_Deep)
{
    processLines("{\n{\n{\ncmd1\n}\n}\n}\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\n");
}

TEST_F(CommandParserTest, NestedDynamicBlock_MultipleNested)
{
    processLines("{\ncmd1\n{\ncmd2\n}\n{\ncmd3\n}\ncmd4\n}\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1, cmd2, cmd3, cmd4\n");
}

TEST_F(CommandParserTest, Error_UnexpectedClosingBracket)
{
    EXPECT_THROW(parser.process("}"), std::runtime_error);
}

TEST_F(CommandParserTest, Error_MultipleClosingBrackets)
{
    parser.process("{");
    parser.process("cmd1");
    parser.process("}");
    EXPECT_THROW(parser.process("}"), std::runtime_error);
}

TEST_F(CommandParserTest, Error_UnclosedDynamicBlock)
{
    parser.process("cmd1");
    parser.process("{");
    parser.process("cmd2");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\n");
}

TEST_F(CommandParserTest, EdgeCase_EmptyInput)
{
    parser.end_parse();
    EXPECT_TRUE(printer.get_output().empty());
}

TEST_F(CommandParserTest, EdgeCase_OnlyBrackets)
{
    processLines("{\n}\n");
    parser.end_parse();
    EXPECT_TRUE(printer.get_output().empty());
}

TEST_F(CommandParserTest, EdgeCase_StaticBlockInterrupted)
{
    processLines("cmd1\ncmd2\n{\ncmd3\n}\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1, cmd2\ncmd3\n");
}

TEST_F(CommandParserTest, EdgeCase_DynamicToStaticTransition)
{
    processLines("{\ncmd1\n}\ncmd2\ncmd3\ncmd4\n");
    parser.end_parse();
    EXPECT_EQ(printer.get_output(), "cmd1\ncmd2, cmd3, cmd4\n");
}

TEST_F(CommandParserTest, EdgeCase_SameBlockSizeOne)
{
    CommandParser parser1{1};
    TestPrinter printer1;
    parser1.add_printer(&printer1);

    std::istringstream stream("cmd1\ncmd2\ncmd3\n");
    std::string line;
    while (std::getline(stream, line))
    {
        parser1.process(line);
    }
    parser1.end_parse();

    EXPECT_EQ(printer1.get_output(), "cmd1\ncmd2\ncmd3\n");
}

TEST_F(CommandParserTest, MultiplePrinters)
{
    TestPrinter printer2;
    parser.add_printer(&printer2);

    processLines("cmd1\ncmd2\ncmd3\n");
    parser.end_parse();

    EXPECT_EQ(printer.get_output(), "cmd1, cmd2, cmd3\n");
    EXPECT_EQ(printer2.get_output(), "cmd1, cmd2, cmd3\n");
}

TEST_F(CommandParserTest, MultiplePrinters_DynamicBlock)
{
    TestPrinter printer2;
    parser.add_printer(&printer2);

    processLines("cmd1\n{\ncmd2\ncmd3\n}\n");
    parser.end_parse();

    EXPECT_EQ(printer.get_output(), "cmd1\ncmd2, cmd3\n");
    EXPECT_EQ(printer2.get_output(), "cmd1\ncmd2, cmd3\n");
}