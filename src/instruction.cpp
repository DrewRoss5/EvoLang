#include <optional>
#include "../inc/value.hpp"
#include "../inc/instruction.hpp"

Instruction::Instruction(InstructionType op_code){
    this->op_code = op_code;
}

Instruction::Instruction(InstructionType op_code, const Value& arg_val){
    this->op_code = op_code;
    this->arg = arg_val;
}