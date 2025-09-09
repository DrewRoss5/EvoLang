#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType{
    INT_T,
    STR_T,
    CHAR_T,
    BOOL_T,
    INST_T,
    WORD_T,
    LABEL_T,
    NULL_T
};

struct Token{
    TokenType type;
    std::string text;
    Token(TokenType type, const std::string text) {this->type = type; this->text = text;}
};

#endif