#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <optional>
#include "../inc/value.hpp"

enum class InstructionType{
    INST_NULL,
    INST_PUSH,
    INST_POP,
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
    INST_EQ,
    INST_NEQ,
    INST_JUMP,
    INST_JUMPIF,
    INST_CALL,
    INST_RET,
    INST_GET,
    INST_SET,
    INST_PRINT,
    INST_READ,
    INST_READINT
};

struct Instruction{
    InstructionType op_code;
    std::optional<Value> arg;
    Instruction() : op_code(InstructionType::INST_NULL) {};
    Instruction(InstructionType op_code);
    Instruction(InstructionType op_code, const Value& arg);
};

#endif