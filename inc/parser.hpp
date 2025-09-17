#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../inc/token.hpp"
#include "../inc/instruction.hpp"


class Parser{
    private:
        std::vector<Token> _tokens;
        std::vector<Instruction> _instructions;
        std::vector<std::string> _word_stack;
        std::vector<std::string> _vars;
        std::unordered_map<std::string, int> _labels;
        std::vector<size_t> _jump_indexes;
        size_t _inst_no {0};
        size_t _line_no {0};
        void parse_literal(const Token& token);
        void parse_word(const Token& token);
        void parse_inst(const Token& token);
        void parse_label(const Token& token);
        void parse_type(const Token& token);
    public:
        Parser() {}
        Parser(const std::vector<Token>& tokens) : _tokens(tokens) {}
        void set_tokens(const std::vector<Token>& tokens);
        void reset();
        void reset(const std::vector<Token>& tokens);
        std::vector<Instruction> parse_expr(bool clear = false);
        std::vector<Instruction> parse_program(std::vector<std::vector<Token>>& tokens);
};

#endif