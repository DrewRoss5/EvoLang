#ifndef LEXER_H
#define LEXER_H 

#include <string>
#include <sstream>
#include <vector>

#include "../inc/token.hpp"

std::vector<Token> tokenize_expr(std::string& expr);
std::vector<std::vector<Token>> tokenize_program(std::stringstream& ss);

#endif
