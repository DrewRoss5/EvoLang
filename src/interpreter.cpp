#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <format>
#include "../inc/parser.hpp"
#include "../inc/value.hpp"
#include "../inc/lexer.hpp"
#include "../inc/instruction.hpp"
#include "../inc/interpreter.hpp"



// STACK INSTRUCTIONS FOLLOW
// pushes a value on to the top of stack
void Interpreter::stack_push(const Value& val){
    this->_stack.push_back(val);
}

// duplicates the top value of the stack
void Interpreter::stack_dup(){
    if (this->_stack.empty())
        throw std::runtime_error(std::format("Error on line {}: cannot retrieve a value from an empty stack.", this->_line_no));
    Value val = this->_stack.back();
    this->_stack.push_back(val);
}

// CALL STACK FUNTIONS FOLLOW
// pops the top address off the call stack, or returns 0 if empty
size_t Interpreter::_pop_return(){
    if (this->_return_addrs.empty())
        return 0;
    size_t ret_val = _return_addrs.back();
    _return_addrs.pop_back();
    return ret_val;
}

void Interpreter::_push_return(size_t addr){
    this->_return_addrs.push_back(addr);
}

// returns a constant reference to the top value of the stack
const Value& Interpreter::stack_top(){
    if (this->_stack.empty())
        throw std::runtime_error(std::format("Error on line {}: cannot retrieve a value from an empty stack.", this->_line_no));
    return this->_stack.back();
}

// pops the top value off the stack and returns it
Value Interpreter::stack_pop(){
    if (this->_stack.empty())
        throw std::runtime_error(std::format("Error on line {}: cannot retrieve a value from an empty stack.", this->_line_no));
    Value val = this->_stack.back();
    this->_stack.pop_back();
    return val;
}

// PROGRAM INSTRUCTIONS FOLLOW
// runs an arithmetic operation
void Interpreter::_arith_op(const Instruction& inst){
    // ensure there at least two values on the stack to pop
    if (this->_stack.size() < 2)
        throw std::runtime_error(std::format("Error on line {}: arithmetic operations require at least two values on the stack", this->_line_no));
    Value right_val = this->stack_pop();
    Value left_val = this->stack_pop();
    // ensure values match and are of the right type
    if (right_val.get_type() != left_val.get_type())
        throw std::runtime_error(std::format("Error on line {}: arithmetic cannot be performed on mismatch types", this->_line_no));
    if (right_val.get_type() != ValueType::TYPE_INT)
        throw std::runtime_error(std::format("Error on line {}: invalid type for arithmetic operation", this->_line_no));
    int rhs {std::get<int>(right_val.get_value())}, lhs {std::get<int>(left_val.get_value())}, retval;
    switch (inst.op_code){
    case InstructionType::INST_ADD:
        retval = lhs + rhs;
        break;
    case InstructionType::INST_SUB:
        retval = lhs - rhs;
        break;
    case InstructionType::INST_MUL:
        retval = lhs * rhs;
        break;
    case InstructionType::INST_DIV:
        retval = lhs / rhs;
        break;
    case InstructionType::INST_MOD:
        retval = lhs % rhs;
        break;
    }
    Value result = Value(ValueType::TYPE_INT, retval);
    this->stack_push(result);
}

// runs a logical operation
void Interpreter::_logic_op(const Instruction& inst){
    // ensure there at least two values on the stack to pop
    if (this->_stack.size() < 2)
        throw std::runtime_error(std::format("Error on line {}: logical operations require at least two values on the stack", this->_line_no));
    Value right_val = this->stack_pop();
    Value left_val = this->stack_pop();
    // ensure values match and are of an integral type
    if (right_val.get_type() != left_val.get_type())
        throw std::runtime_error(std::format("Error on line {}: logical operations cannot be performed on mismatch types", this->_line_no));
    if (!right_val.is_intergral())
        throw std::runtime_error(std::format("Error on line {}: invalid type for logical operation", this->_line_no));
    int lhs {left_val.as_int()}, rhs {right_val.as_int()}, retval;
    bool eqval;
    switch (inst.op_code){
        case InstructionType::INST_AND:
            retval = lhs & rhs;
            break;
        case InstructionType::INST_OR:
            retval = lhs | rhs;
            break;
        case InstructionType::INST_XOR:
            retval = lhs ^ rhs;
            break;
    }  
    this->stack_push(Value::from_int(left_val.get_type(), retval));
}

// runs a coparison operation
void Interpreter::_comp_op(const Instruction& inst){
    if (this->_stack.size() < 2)
        throw std::runtime_error(std::format("Error on line {}: comparison operations require at least two values on the stack", this->_line_no));
    Value right_val = this->stack_pop();
    Value left_val = this->stack_pop();
    int comparison_offset {16};
    bool result;
    switch (inst.op_code){
        case InstructionType::INST_NEQ:
        case InstructionType::INST_EQ:
            // this is some black magic
            result = (left_val == right_val) == static_cast<bool>(static_cast<int>(inst.op_code) - 14);
            this->stack_push(Value(ValueType::TYPE_BOOL, result));
            return;
        case InstructionType::INST_LESS:
        case InstructionType::INST_GREATER:
        case InstructionType::INST_LESS_EQ:
        case InstructionType::INST_GREATER_EQ:
            if (!left_val.is_intergral() || !right_val.is_intergral())
                throw std::runtime_error(std::format("Error on line {}: comparison operations cannot be performed on non-integral types", this->_line_no));
            // check if this is a "or equal operation"
            if (inst.op_code > InstructionType::INST_GREATER){
                if (left_val == right_val){
                    this->stack_push(Value(ValueType::TYPE_BOOL, true));
                    return;
                }
                comparison_offset += 2;  
            }
            // similar black magic to above
            result = (left_val > right_val) == static_cast<bool>(static_cast<int>(inst.op_code) - comparison_offset);
            this->stack_push(Value(ValueType::TYPE_BOOL, result));
            break;
    }
}

// runs a logical not operation 
void Interpreter::_not_op(const Instruction& inst){
    // ensure there is at least one item on the stack
    if (this->_stack.size() < 1)
        throw std::runtime_error(std::format("Error on line {}: No stack data for not operation", this->_line_no));
    Value val = this->stack_pop();
    if (!val.is_intergral())
        throw std::runtime_error(std::format("Error on line {}: invalid type for NOT operation", this->_line_no));
    int data = val.as_int();
    bool result = (data != 0);
    this->stack_push(Value(ValueType::TYPE_BOOL, result));
}

// runs a jump operation
void Interpreter::_jump_op(const Instruction& inst){
    Value condition_val;
    int addr;
    switch (inst.op_code){
        case InstructionType::INST_JUMP:
        case InstructionType::INST_CALL:
            if (inst.op_code == InstructionType::INST_CALL)
                this->_push_return(this->_next_op);
            this->_next_op = std::get<int>(inst.arg.value().get_value()) - 1;
            break;
        case InstructionType::INST_RET:
            // no validation is needed, as the return address can only be set by the above case, if no address is set, this will restart the program
            this->_next_op = _pop_return();
            break;
        case InstructionType::INST_JUMPIF:
            if (this->_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: no value to evaluate for jif instruction", this->_line_no));
            condition_val = this->stack_pop();
            if (condition_val.as_int())
                this->_next_op = std::get<int>(inst.arg.value().get_value()) - 1;
            break;
    }
}

// runs set and get operations
void Interpreter::_var_op(const Instruction& inst){
    Value val;
    std::string var_name;
    switch (inst.op_code){
        case InstructionType::INST_SET:
            if (this->_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: Not enough stack data to assign variable", this->_line_no));
            val = this->stack_pop();
            this->_vars[std::get<std::string>(inst.arg.value().get_value())] = val;
            break;
        case InstructionType::INST_GET:
            var_name = std::get<std::string>(inst.arg.value().get_value());
            if (!this->_vars.count(var_name))
                throw std::runtime_error(std::format("Error on line {}: Variable \"{}\" is undeclared", this->_line_no, var_name));
            val = this->_vars.at(var_name);
            this->stack_push(val);
            break;
    }
}

// runs an I/O operations
void Interpreter::_io_op(const Instruction& inst){
    Value val;
    std::string str_in;
    int num_in;
    switch (inst.op_code){
        case InstructionType::INST_PRINT:
            if (this->_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: Not enough stack data to print", this->_line_no));
            val = this->stack_top();
            std::cout << val.to_string();
            break;
        case InstructionType::INST_PRINTLN:
            if (this->_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: Not enough stack data to print", this->_line_no));
            val = this->stack_top();
            std::cout << val.to_string() << std::endl;
            break;
        case InstructionType::INST_READ:
            std::getline(std::cin, str_in);
            this->stack_push(Value(ValueType::TYPE_STR, str_in));
            break;
        case InstructionType::INST_READINT:
            std::getline(std::cin, str_in);
            try{
                num_in = std::stoi(str_in);
                this->stack_push(Value(ValueType::TYPE_INT, num_in));
            }
            catch (const std::invalid_argument & e) {
                throw std::runtime_error(std::format("Error on line {}: Non-integer input recived for readint", this->_line_no));
            }
            catch (const std::out_of_range & e) {
                throw std::runtime_error(std::format("Error on line {}: Out-of-range input recived for readint", this->_line_no));
            }
            break;
    }
}

// runs an array/collection operation
void Interpreter::_arr_op(const Instruction& inst){
    Value collection, index, result;
    int length;
    switch (inst.op_code){
        case InstructionType::INST_AT:
            if (this->_stack.size() < 2)
                throw std::runtime_error(std::format("Error on line {}: The \"at\" instruction expects at least two values on the stack.", this->_line_no));
            collection = this->stack_pop();
            index = this->stack_pop();
            if (index.get_type() != ValueType::TYPE_INT)
                throw std::runtime_error(std::format("Error on line {}: Index values must be of integer type", this->_line_no));
            try{
                result = collection.get_index(index.as_int());
                this->stack_push(result);
            }
            catch (std::out_of_range e){
                throw std::runtime_error(std::format("Range Error on line {}: Index out of range.", this->_line_no));
            }
            catch (std::runtime_error e){
                throw std::runtime_error(std::format("Error on line {}: {}", this->_line_no, e.what()));
            }
            break;
        case InstructionType::INST_LEN:
            if (this->_stack.size() < 1)
                throw std::runtime_error(std::format("Error on line {}: The \"len\" instruction expects at least one value on the stack.", this->_line_no));
            collection = this->stack_pop();
            try{
                length = collection.get_len();
                result = Value(ValueType::TYPE_INT, length);
                this->stack_push(result);
            }
            catch (std::runtime_error e){
                throw std::runtime_error(std::format("Error on line {}: {}", this->_line_no, e.what()));
            }
            break;
    }
}

// runs a type or conversion operation
void Interpreter::_type_op(const Instruction& inst){
    int int_val;
    Value val, type, result;
    switch (inst.op_code){
        case InstructionType::INST_TYPE:
            if (this->_stack.empty())
                throw std::runtime_error(std::format("Error on line {}: insufficient stack data for 'type' command", this->_line_no));
            val = this->stack_pop();
            result = Value(ValueType::TYPE_VALTYPE, static_cast<int>(val.get_type()));
            break;
        case InstructionType::INST_CONVERT:
            if (this->_stack.size() < 2)
                throw std::runtime_error(std::format("Error on line {}: insufficient stack data for 'conv' command", this->_line_no));
            type = this->stack_pop();
            val = this->stack_pop();
            if (type.get_type() != ValueType::TYPE_VALTYPE)
                throw std::runtime_error(std::format("Error on line {}: Invalid type for conversion", this->_line_no));
            try{
                switch (static_cast<ValueType>(std::get<int>(type.get_value()))){
                    case ValueType::TYPE_INT:
                        // strings are non-integral values, and must be handled seprately
                        if (val.get_type() == ValueType::TYPE_STR){
                            int_val = std::stoi(std::get<std::string>(val.get_value()));
                            result = Value(ValueType::TYPE_INT, int_val);
                        }
                        else
                            result = Value(ValueType::TYPE_INT, val.as_int());
                        break;
                    case ValueType::TYPE_CHAR:
                        result = Value(ValueType::TYPE_CHAR, val.as_char());
                        break;
                    case ValueType::TYPE_BOOL:
                        result = Value(ValueType::TYPE_BOOL, val.as_bool());
                        break;
                    case ValueType::TYPE_STR:
                        result = Value(ValueType::TYPE_STR, val.to_string());
                        break;
                }
            }
            catch (std::runtime_error e){
                throw std::runtime_error(std::format("Error on line {}: {}", this->_line_no, e.what()));
            }
            catch (const std::invalid_argument & e) {
                throw std::runtime_error(std::format("Error on line {}: Non-integer input recived for readint", this->_line_no));
            }
            catch (const std::out_of_range & e) {
                throw std::runtime_error(std::format("Error on line {}: Out-of-range input recived for readint", this->_line_no));
            }
            break;
    }
    this->stack_push(result);
}

// INTERPRETER FUNCTIONS FOLLOW
// runs a list of instrunctions produced by the parser
void Interpreter::_run_bytecode(){
    Instruction inst;
    while (this->_next_op < this->_instructions.size()){
        inst = this->_instructions[this->_next_op];
        switch (inst.op_code){
            case InstructionType::INST_POP:
                this->stack_pop();
                break;
            case InstructionType::INST_DUP:
                this->stack_dup();
                break;
            case InstructionType::INST_PUSH:
                if (!inst.arg.has_value())
                    throw std::runtime_error(std::format("Error on line {}: illegal instruction", this->_line_no));
                this->stack_push(inst.arg.value());
                break;
            case InstructionType::INST_SIZE:
                this->stack_push(Value(ValueType::TYPE_INT, static_cast<int>(this->_stack.size())));
                break;
            case InstructionType::INST_ADD:
            case InstructionType::INST_SUB:
            case InstructionType::INST_MUL:
            case InstructionType::INST_DIV:
            case InstructionType::INST_MOD:
                this->_arith_op(inst);
                break;
            case InstructionType::INST_AND:
            case InstructionType::INST_OR:
            case InstructionType::INST_XOR:
                this->_logic_op(inst);
                break;
            case InstructionType::INST_EQ:
            case InstructionType::INST_NEQ:
            case InstructionType::INST_LESS:
            case InstructionType::INST_GREATER:
            case InstructionType::INST_LESS_EQ:
            case InstructionType::INST_GREATER_EQ:
                this->_comp_op(inst);
                break;
            case InstructionType::INST_NOT:
                this->_not_op(inst);
                break;
            case InstructionType::INST_JUMP:
            case InstructionType::INST_JUMPIF:
            case InstructionType::INST_CALL:
            case InstructionType::INST_RET:
                this->_jump_op(inst);
                break;
            case InstructionType::INST_GET:
            case InstructionType::INST_SET:
                this->_var_op(inst);
                break;
            case InstructionType::INST_PRINT:
            case InstructionType::INST_PRINTLN:
            case InstructionType::INST_READ:
            case InstructionType::INST_READINT:
                this->_io_op(inst);
                break;
            case InstructionType::INST_AT:
            case InstructionType::INST_LEN:
                this->_arr_op(inst);
                break;
            case InstructionType::INST_TYPE:
            case InstructionType::INST_CONVERT:
                this->_type_op(inst);
                break;
        }
        this->_next_op++;
    }
}

// runs a single expression, and returns the top value remaining on the stack, or an empty value if the stack is empty
Value Interpreter::run_expr(std::string expr){
    this->_next_op = 0;
    std::vector<Token> tokens = tokenize_expr(expr);
    this->_parser.set_tokens(tokens);
    this->_instructions = this->_parser.parse_expr(true);
    this->_run_bytecode();
    if (this->_stack.empty())
        return Value(ValueType::TYPE_NULL, "");
    return this->stack_top();
}

// runs a multiline program, treating each line as an expression. returns the top value remaining on the stack, or an empty value if the stack is empty
// TODO: Make this keep track of line number
Value Interpreter::run_prog(std::stringstream& program){
    this->_next_op = 0;
    std::vector<std::vector<Token>> tokens = tokenize_program(program);
    this->_parser.reset();
    this->_instructions = this->_parser.parse_program(tokens);
    this->_run_bytecode();
    if (this->_stack.empty())
        return Value(ValueType::TYPE_NULL, "");
    return this->stack_pop();
}

// resets the interpreter's state
void Interpreter::reset_state(){
    this->_return_addrs.clear();
    this->_line_no = 0;
    this->_next_op = 0;
    this->_parser.reset();
    this->_instructions.clear();
    this->_vars.clear();
}