#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <optional>
#include "../inc/value.hpp"

enum class InstructionType{
    INST_NULL,
    INST_PUSH,
    INST_POP,
    INST_CLEAR,
    INST_PEEK,
    INST_SWAP,
    INST_SIZE,
    INST_DUP,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD,
    INST_AND,
    INST_OR,
    INST_XOR,
    INST_NOT,
    INST_NEQ,
    INST_EQ,
    INST_LESS,
    INST_GREATER,
    INST_LESS_EQ,
    INST_GREATER_EQ,
    INST_JUMP,
    INST_JUMPIF,
    INST_CALL,
    INST_RET,
    INST_GET,
    INST_SET,
    INST_PRINT,
    INST_PRINTLN,
    INST_READ,
    INST_READINT,
    INST_AT,
    INST_LEN,
    INST_TYPE,
    INST_CONVERT
};

struct Instruction{
    InstructionType op_code;
    std::optional<Value> arg;
    Instruction() : op_code(InstructionType::INST_NULL) {};
    Instruction(InstructionType op_code);
    Instruction(InstructionType op_code, const Value& arg);
    void set_arg(const Value& new_arg) {this->arg = new_arg;}
};

#endif