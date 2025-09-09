#include <string>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <format>

#include "../inc/token.hpp"
#include "../inc/parser.hpp"

// associates each instruction keyword with it's op code
const std::unordered_map<std::string, InstructionType> inst_map = {
    {"push", InstructionType::INST_PUSH},
    {"pop", InstructionType::INST_POP},
    {"dup", InstructionType::INST_DUP},
    {"add", InstructionType::INST_ADD},
    {"sub", InstructionType::INST_SUB},
    {"mul", InstructionType::INST_MUL},
    {"div", InstructionType::INST_DIV},
    {"mod", InstructionType::INST_MOD},
    {"and", InstructionType::INST_AND},
    {"or", InstructionType::INST_OR},
    {"xor", InstructionType::INST_XOR},
    {"not", InstructionType::INST_NOT},
    {"eq", InstructionType::INST_EQ},
    {"neq", InstructionType::INST_NEQ},
    {"j", InstructionType::INST_JUMP},
    {"jif", InstructionType::INST_JUMPIF},
    {"call", InstructionType::INST_CALL},
    {"ret", InstructionType::INST_RET},
    {"set", InstructionType::INST_SET},
    {"get", InstructionType::INST_GET},
    {"print", InstructionType::INST_PRINT},
    {"println", InstructionType::INST_PRINTLN},
    {"print_p", InstructionType::INST_PRINT},
    {"println_p", InstructionType::INST_PRINTLN},
    {"read", InstructionType::INST_READ},
    {"readint", InstructionType::INST_READINT}
};

// parses a literal expression, evaluates the value and creates a push instruction for it 
void Parser::parse_literal(const Token& token){
    Value val;
    bool bool_val;
    int int_val;
    switch (token.type){
        case TokenType::BOOL_T:
            bool_val = (token.text == "TRUE");
            val = Value(ValueType::TYPE_BOOL, bool_val);
            break;
        case TokenType::INT_T:
            int_val = std::stoi(token.text);
            val = Value(ValueType::TYPE_INT, int_val);
            break;
        case TokenType::CHAR_T:
            val = Value(ValueType::TYPE_CHAR, token.text[0]);
            break;
        case TokenType::STR_T:
            val = Value(ValueType::TYPE_STR, token.text);
            break;
    }
    this->_instructions.emplace_back(InstructionType::INST_PUSH, val);
    this->_inst_no++;
}

// parses a non-keyword name, checking the next token to determine how to handle it
void Parser::parse_word(const Token& token){
    // the next instruction is "set" and needs only the variable name
    if (!this->_tokens.empty() && this->_tokens.back().text == "set"){
        this->_word_stack.push_back(token.text);
    }
    // the next instruction is get and we need to determine if the variable is known to exist
    else if (!this->_tokens.empty() && this->_tokens.back().text == "get"){
        // ensure the variable has been declared
        if (std::find(this->_vars.begin(), this->_vars.end(), token.text) == this->_vars.end())
            throw std::runtime_error(std::format("Error on line {}: use of undeclared varaible \"{}\"" , this->_line_no, token.text));
        this->_word_stack.push_back(token.text);
    }
    // the next token is unknown, and we assume this is an implicit get if its a variable, otherwise, we assume that it's a label
    else{
        // parse the word as a variable if declared
        if (std::find(this->_vars.begin(), this->_vars.end(), token.text) != this->_vars.end()){
            Value name_val(ValueType::TYPE_NAME, token.text);
            this->_instructions.emplace_back(InstructionType::INST_GET, name_val);
            this->_inst_no++;
        }
        // parse the word as a label (simply push it to the word stack)
        this->_word_stack.push_back(token.text);
    }
}

// parses a named instruction
void Parser::parse_inst(const Token& token){
    InstructionType op_code = inst_map.at(token.text);
    Value arg_val;
    std::string var_name, label_name;
    switch (op_code){
        case InstructionType::INST_PUSH:
            // at the moment, the explicit PUSH command is only sugar, so we can ignore it, as values are already implicitly pushed
            break;
        case InstructionType::INST_GET:
            // ensure there is a variable found
            if (_word_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: expected an identifier" , this->_line_no));
            var_name = this->_word_stack.back();
            this->_word_stack.pop_back(); 
            // ensure the variable has been decleared
            if (std::find(this->_vars.begin(), this->_vars.end(), token.text) == this->_vars.end())
                throw std::runtime_error(std::format("Error on line {}: use of undeclared varaible \"{}\"" , this->_line_no, token.text));
            arg_val = Value(ValueType::TYPE_NAME, var_name);
            this->_instructions.emplace_back(op_code, arg_val);
            this->_inst_no++;
            break;
        case InstructionType::INST_SET:
            if (_word_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: expected an identifier" , this->_line_no));
            var_name = this->_word_stack.back();
            this->_word_stack.pop_back(); 
            arg_val = Value(ValueType::TYPE_NAME, var_name);
            this->_instructions.emplace_back(InstructionType::INST_SET, arg_val);
            this->_inst_no++;
            // add the variable name to the list of declared variables if not already decleared
            if (std::find(this->_vars.begin(), this->_vars.end(), token.text) == this->_vars.end())
                this->_vars.push_back(var_name);
            break;
        case InstructionType::INST_JUMP:
        case InstructionType::INST_CALL:
        case InstructionType::INST_JUMPIF:
            if (this->_word_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: Jump statement must have label", this->_line_no));
            label_name = this->_word_stack.back();
            this->_word_stack.pop_back();
            // check if the label is already defined, and assign the direct instruction number if so
            if (this->_labels.count(token.text))
                arg_val = Value(ValueType::TYPE_INT, this->_labels[label_name]);
            else
                arg_val = Value(ValueType::TYPE_STR, label_name);
            this->_instructions.emplace_back(op_code, arg_val);
            this->_inst_no++;
            break;
        case InstructionType::INST_PRINT:
        case InstructionType::INST_PRINTLN:
            this->_instructions.emplace_back(op_code);
            this->_inst_no++;
            // check if this is a print and pop command
            if (token.text.ends_with("_p")){
                this->_instructions.emplace_back(InstructionType::INST_POP);
                this->_inst_no++;
            }
            break;
        default:
            // the instruction is a "simple" instruction which takes no argument
            this->_instructions.emplace_back(op_code);
            this->_inst_no++;
            break;

    }
}

void Parser::parse_label(const Token& token){
    if (this->_labels.count(token.text))
        throw std::runtime_error(std::format("Error on line {}: redeclaration of label \"{}\"" , this->_line_no, token.text));
    this->_labels[token.text] = this->_inst_no - 1;
}

// parses the instructions for a single expression in reverse ordeer
std::vector<Instruction> Parser::parse_expr(){
    Token token{TokenType::NULL_T, ""};
    while (!this->_tokens.empty()){
        token = this->_tokens.back();
        this->_tokens.pop_back();
        switch (token.type){
            case TokenType::BOOL_T:
            case TokenType::INT_T:
            case TokenType::CHAR_T:
            case TokenType::STR_T:
                this->parse_literal(token);
                break;
            case TokenType::WORD_T:
                this->parse_word(token);
                break;
            case TokenType::INST_T:
                this->parse_inst(token);
                break;
            case TokenType::LABEL_T:
                this->parse_label(token);
                break;
        }
    }
    return this->_instructions;
}

// parses a program, each token vector represents a single expression
std::vector<Instruction> Parser::parse_program(std::vector<std::vector<Token>>& statements){
    for (std::vector<Token>& statement : statements){
        _line_no++;
        this->_tokens = statement;
        this->parse_expr();
    }
    // read through all instructions to find if there are any jumps with unresolved labels, and resolve them if so
    // TODO: Find a more efficient way to do this
    Value label_no;
    std::string label_str;
    for (int i = 0; i < this->_instructions.size(); i++){
        if (this->_instructions[i].op_code == InstructionType::INST_JUMP || this->_instructions[i].op_code == InstructionType::INST_CALL || this->_instructions[i].op_code == InstructionType::INST_JUMPIF)
            if (this->_instructions[i].arg.value().get_type() == ValueType::TYPE_STR){
                label_str = std::get<std::string>(this->_instructions[i].arg.value().get_value());
                if (!this->_labels.count(label_str))
                    throw std::runtime_error(std::format("Error: use of undeclared label"));
                label_no = Value(ValueType::TYPE_INT, this->_labels[label_str]);
                this->_instructions[i].set_arg(label_no);
            }
    }
    return this->_instructions;
}

// resets the parser's state entirely
void Parser::reset(){
    this->_line_no = 0;
    this->_inst_no = 0;
    this->_vars.clear();
    this->_word_stack.clear();
    this->_instructions.clear();
    this->_tokens.clear();
}


// resets the parser's state, and sets its tokens to the provided vector
void Parser::reset(const std::vector<Token>& tokens){
    this->_line_no = 0;
    this->_inst_no = 0;
    this->_vars.clear();
    this->_word_stack.clear();
    this->_instructions.clear();
    this->_tokens = tokens;
}
