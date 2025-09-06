#include <iostream>
#include <cassert>
#include "../inc/lexer.hpp"

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
    std::vector<Token> instructions = tokenize_expr(expr2);
    assert(instructions.size() == 3);
    assert(instructions[0].type == TokenType::INST_T && instructions[0].text == "push");
    assert(instructions[1].type == TokenType::INST_T && instructions[1].text == "pop");
    assert(instructions[2].type == TokenType::INST_T && instructions[2].text == "add");
    std::cout << "OK" << std::endl;

    std::cout << "Testing words...";
    std::string expr3 = "something random";
    std::vector<Token> words = tokenize_expr(expr3);
    assert(words[0].type == TokenType::WORD_T && words[0].text == "something");
    std::cout << "OK" << std::endl;

}