#include <map>
#include <stdexcept>
#include <format>

#include "../inc/token.hpp"
#include "../inc/lexer.hpp"

// associate each basic keyword with a token
std::map<std::string, TokenType> token_map = {
    {"push", TokenType::INST_T},
    {"->", TokenType::INST_T},
    {"pop", TokenType::INST_T},
    {"clear", TokenType::INST_T},
    {"dup", TokenType::INST_T},
    {"size", TokenType::INST_T},
    {"add", TokenType::INST_T},
    {"+", TokenType::INST_T},
    {"sub", TokenType::INST_T},
    {"-", TokenType::INST_T},
    {"mul", TokenType::INST_T},
    {"*", TokenType::INST_T},
    {"div", TokenType::INST_T},
    {"/", TokenType::INST_T},
    {"mod", TokenType::INST_T},
    {"%", TokenType::INST_T},
    {"and", TokenType::INST_T},
    {"&", TokenType::INST_T},
    {"or", TokenType::INST_T},
    {"|", TokenType::INST_T},
    {"xor", TokenType::INST_T},
    {"^", TokenType::INST_T},
    {"not", TokenType::INST_T},
    {"!", TokenType::INST_T},
    {"eq", TokenType::INST_T},
    {"==", TokenType::INST_T},
    {"neq", TokenType::INST_T},
    {"!=", TokenType::INST_T},
    {"gt", TokenType::INST_T},
    {">", TokenType::INST_T},
    {"lt", TokenType::INST_T},
    {"<", TokenType::INST_T},
    {"gte", TokenType::INST_T},
    {">=", TokenType::INST_T},
    {"lte", TokenType::INST_T},
    {"<=", TokenType::INST_T},
    {"j", TokenType::INST_T},
    {"jif", TokenType::INST_T},
    {"j==", TokenType::INST_T},
    {"j!=", TokenType::INST_T},
    {"j<", TokenType::INST_T},
    {"j>", TokenType::INST_T},
    {"j<=", TokenType::INST_T},
    {"j>=", TokenType::INST_T},
    {"call", TokenType::INST_T},
    {"ret", TokenType::INST_T},
    {"set", TokenType::INST_T},
    {"<-", TokenType::INST_T},
    {"get", TokenType::INST_T},
    {"print", TokenType::INST_T},
    {"println", TokenType::INST_T},
    {"print_p", TokenType::INST_T},
    {"println_p", TokenType::INST_T},
    {"read", TokenType::INST_T},
    {"readint", TokenType::INST_T},
    {"at", TokenType::INST_T},
    {"len", TokenType::INST_T},
    {"conv", TokenType::INST_T},
    {"type", TokenType::INST_T},
    {"TRUE", TokenType::BOOL_T},
    {"FALSE", TokenType::BOOL_T},
    {"int", TokenType::TYPE_T},
    {"bool", TokenType::TYPE_T},
    {"char", TokenType::TYPE_T},
    {"string", TokenType::TYPE_T}
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
        if (tmp.length() > 0)
            out.push_back(tmp);
    }
    // push the last segment (not followed by a space) to the output vector
    if (str.length() > 0)
        out.push_back(str);
    return out;
}

// checks if a string is a number, returns INT_T if the string is an integer, FLOAT_T if the string is a float, and NULL_T if the string is non-numeric
TokenType num_type(const std::string& str){
    bool radix_encountered {false};
    TokenType ret_type {TokenType::INT_T};
    // ensure each character is a digit between 0 and 9, or '.'
    char chr;
    for (int i = 0; i < str.size(); i++){
        chr = str[i];
        if (chr == '.'){
            // check if this is the first radix we've seen, and that it's followed by something
            if (!radix_encountered && ((i + 1) < str.size())){
                radix_encountered = true;
                ret_type = TokenType::FLOAT_T;
            }
            else
                return TokenType::NULL_T;
        }
        else if (chr < '0' || chr > '9') 
            return TokenType::NULL_T;
    }
    return ret_type;
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
        // check if the word is a numeric literal
        TokenType str_num = num_type(word);
        if (str_num != TokenType::NULL_T)
            tokens.emplace_back(str_num, word);
        // check if the word is a string and parse it if so
        else if (word[0] == '"'){
            // word size checked here to account for the possibility of a string starting with 
            while (word.back() != '"' || word.size() == 1){
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
            if (word.size() == 3 && word.back() == '\'')
                tmp.text.push_back(word[1]);
            // check if the character is a space
            else if (word.size() == 1 &&(i + 1) < words.size() && words[i + 1] == "'"){
                tmp.text = " ";
                i++;
            }
            else
                throw (std::runtime_error("Invalid character literal"));
            tokens.emplace_back(tmp);
        }
        else if (word.back() == ':'){
            tokens.emplace_back(TokenType::LABEL_T, word.substr(0, word.size() - 1));
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