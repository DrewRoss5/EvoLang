#include <map>
#include <stdexcept>
#include <format>

#include "../inc/token.hpp"
#include "../inc/lexer.hpp"

// associate each basic keyword with a token
std::map<std::string, TokenType> token_map = {
    {"push", TokenType::INST_T},
    {"pop", TokenType::INST_T},
    {"dup", TokenType::INST_T},
    {"add", TokenType::INST_T},
    {"sub", TokenType::INST_T},
    {"mul", TokenType::INST_T},
    {"div", TokenType::INST_T},
    {"mod", TokenType::INST_T},
    {"and", TokenType::INST_T},
    {"or", TokenType::INST_T},
    {"xor", TokenType::INST_T},
    {"not", TokenType::INST_T},
    {"eq", TokenType::INST_T},
    {"neq", TokenType::INST_T},
    {"j", TokenType::INST_T},
    {"jif", TokenType::INST_T},
    {"call", TokenType::INST_T},
    {"ret", TokenType::INST_T},
    {"set", TokenType::INST_T},
    {"get", TokenType::INST_T},
    {"print", TokenType::INST_T},
    {"println", TokenType::INST_T},
    {"read", TokenType::INST_T},
    {"readint", TokenType::INST_T},
    {"TRUE", TokenType::BOOL_T},
    {"FALSE", TokenType::BOOL_T}
};

// splits a string by a given delimter character (default space)
std::vector<std::string> split_str(std::string& str, char delim = ' '){
    std::vector<std::string> out;
    size_t delim_pos;
    std::string tmp;
    while (true){
        delim_pos =  str.find(delim);
        if (delim_pos == str.npos)
            break;
        tmp = str.substr(0, delim_pos);
        str = str.substr(delim_pos + 1);
        out.push_back(tmp);
    }
    // push the last segment (not followed by a space) to the output vectpr
    out.push_back(str);
    return out;
}

// checks if a string is an integer
bool is_int(const std::string str){
    // ensure each character is a digit between 0 and 9
    for (char chr : str){
        if (chr < '0' || chr > '9')
            return false;
    }
    return true;
}

// ensures a character literal is valid, and returns the character
char parse_char(const std::string& lit){
    if (lit.size() != 3 || lit[2] != '\'' || lit[1] == '\'')
        throw std::runtime_error("Invalid character literal");
    return lit[1];
}

// parses a single line expression, and returns its tokens
std::vector<Token> tokenize_expr(std::string& expr){
    std::vector<std::string> words = split_str(expr);
    std::vector<Token> tokens;
    std::string word;
    for (int i = 0; i < words.size(); i++){
        word = words[i];
        // exit early if we see the comment marker 
        if (word[0] == '#')
            return tokens;
        // check if the word is a predefined token
        if (token_map.count(word)){
            tokens.emplace_back(token_map[word], word);
            continue;
        }
        // check if the word is a literal
        if (is_int(word))
            tokens.emplace_back(TokenType::INT_T, word);
        // check if the word is a string and parse it if so
        else if (word[0] == '"'){
            while (word.back() != '"'){
                // we're at the end of the statement with an unterminated string literal
                if (word.size() == (i + 1))
                    throw std::runtime_error("Unterminated string literal");
                i++;
                word += " " + words[i];
            }
            // remove quotations
            word = word.substr(1, word.size() - 2);
            tokens.emplace_back(TokenType::STR_T, word);
        }
        // check if the word is a charater and parse it if so
        else if (word[0] == '\''){
            Token tmp(TokenType::CHAR_T, "");
            tmp.text.push_back(parse_char(word));
            tokens.push_back(tmp);
        }
        // all other tokens are generic "words", to be handled by the parser
        else
            tokens.emplace_back(TokenType::WORD_T, word);
    }
    return tokens;
}

std::vector<std::vector<Token>> tokenize_program(std::stringstream& ss){
    std::vector<std::vector<Token>> expressions;
    std::string expr;
    unsigned int line_no = 1;
    while (std::getline(ss, expr)){
        try{
            expressions.push_back(tokenize_expr(expr));
        }
        catch (const std::runtime_error& e){
            throw std::format("Syntax error on line {}: {}", line_no, e.what());
        }
        line_no++;
    }
    return expressions;
}