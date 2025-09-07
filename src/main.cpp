#include <iostream>
#include <cassert>
#include <sstream>
#include "../inc/lexer.hpp"
#include "../inc/parser.hpp"

int main(){
    // a simple lexer test 
    std::cout << "Testing Literals...";
    std::string expr1 = "'a' \"Hello, World!\" 42 TRUE";
    std::vector<Token> literals = tokenize_expr(expr1);
    assert(literals[0].type == TokenType::CHAR_T && literals[0].text == "a");
    assert(literals[1].type == TokenType::STR_T && literals[1].text == "Hello, World!");
    assert(literals[2].type == TokenType::INT_T && literals[2].text == "42");
    assert(literals[3].type == TokenType::BOOL_T && literals[3].text == "TRUE");
    std::cout << "OK" << std::endl;

    std::cout << "Testing Instructions/Comments...";
    std::string expr2 = "push pop add # this is a comment";
    std::vector<Token> token_instructions = tokenize_expr(expr2);
    assert(token_instructions.size() == 3);
    assert(token_instructions[0].type == TokenType::INST_T && token_instructions[0].text == "push");
    assert(token_instructions[1].type == TokenType::INST_T && token_instructions[1].text == "pop");
    assert(token_instructions[2].type == TokenType::INST_T && token_instructions[2].text == "add");
    std::cout << "OK" << std::endl;

    std::cout << "Testing words...";
    std::string expr3 = "something random";
    std::vector<Token> words = tokenize_expr(expr3);
    assert(words[0].type == TokenType::WORD_T && words[0].text == "something");
    std::cout << "OK" << std::endl;

    // a simple parser test
    std::string expr = "add 5 10";
    std::vector<Token> tokens = tokenize_expr(expr);
    Parser parser(tokens);
    std::cout << "Testing basic expression parsing...";
    std::vector<Instruction> instructions = parser.parse_expr();
    assert(instructions.size() == 3);
    assert(instructions[0].op_code == InstructionType::INST_PUSH && std::get<int>(instructions[0].arg.value().get_value()) == 10);
    assert(instructions[1].op_code == InstructionType::INST_PUSH && std::get<int>(instructions[1].arg.value().get_value()) == 5);
    assert(instructions[2].op_code == InstructionType::INST_ADD);
    std::cout << "OK" << std::endl;

    expr = "set foo 5";
    tokens = tokenize_expr(expr);
    parser.reset(tokens);
    std::cout << "Testing variable parsing (setting)...";
    instructions = parser.parse_expr();
    assert(instructions.size() == 2);
    assert(instructions[1].op_code == InstructionType::INST_SET && std::get<std::string>(instructions[1].arg.value().get_value()) == "foo");
    std::cout << "OK" << std::endl;

    expr = "set bar 5\nbar";
    std::stringstream prog_stream(expr);
    std::vector<std::vector<Token>> program_tokens = tokenize_program(prog_stream);
    std::cout << "Testing variable parsing (getting)...";
    parser.reset();
    instructions = parser.parse_program(program_tokens);
    assert(instructions.size() == 3);
    assert(instructions[2].op_code == InstructionType::INST_GET && std::get<std::string>(instructions[1].arg.value().get_value()) == "bar");
    std::cout << "OK" << std::endl;

    std::cout << "All tests passed" << std::endl;
}