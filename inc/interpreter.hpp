#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include "../inc/parser.hpp"
#include "../inc/value.hpp"
#include "../inc/instruction.hpp"

class Interpreter{
    private:
        std::vector<Value> _stack;
        std::vector<Instruction> _instructions;
        std::unordered_map<std::string, Value> _vars;
        Parser _parser;
        size_t _line_no {0};
        size_t _next_op {0};
        std::vector<size_t> _return_addrs;
        size_t _pop_return();
        void _push_return(size_t );
        void _run_bytecode();
        void _stack_op(const Instruction& inst);
        void _arith_op(const Instruction& inst);
        Value _float_op(const Instruction& inst, float rhs, float lhs);
        void _logic_op(const Instruction& inst);
        void _comp_op(const Instruction& inst);
        void _not_op(const Instruction& inst);
        void _jump_op(const Instruction& inst);
        void _var_op(const Instruction& inst);
        void _io_op(const Instruction& inst);
        void _arr_op(const Instruction& inst);
        void _type_op(const Instruction& inst);
        void _cond_op();
    public:
        Value stack_pop();
        void stack_push(const Value& val);
        void stack_dup();
        size_t stack_size() {return this->_stack.size();}
        bool stack_empty() {return this->_stack.empty();}
        const Value& stack_top();
        Value run_expr(std::string expr);
        Value run_prog(std::stringstream& program);
        void reset_state();
};

#endif