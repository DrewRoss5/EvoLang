#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include "../inc/lexer.hpp"
#include "../inc/parser.hpp"
#include "../inc/interpreter.hpp"

Value run_from_file(std::string file_path){
    Interpreter machine;
    std::stringstream buffer;
    std::ifstream prog_file(file_path);
    buffer << prog_file.rdbuf();
    prog_file.close();
    return machine.run_prog(buffer);
}

int main(){
    // a simple interpreter test
    Value result;
    Interpreter machine;
    std::cout << "Testing basic expression...";
    std::string expr = "add 40 2";
    result = machine.run_expr(expr);
    assert(machine.stack_size() == 0);
    assert(std::get<int>(result.get_value()) == 42);
    std::cout << "OK" << std::endl;
    
    std::cout << "Testing variables...";
    expr = "set foo 10\nmul foo 2\nset bar\nsub 5 bar";
    std::stringstream prog_stream(expr);
    result = machine.run_prog(prog_stream);
    assert(machine.stack_size() == 0);
    assert(std::get<int>(result.get_value()) == 15);
    std::cout << "OK" << std::endl;
    std::cout << "Running test program 1, this should display \"Hello, World!\" followed by 666" << std::endl;
    run_from_file("../examples/example1.evo");
    std::cout << "Running test program 2, this should display \"Branch Taken\" followed by \"Branch Not Taken\"" << std::endl;
    run_from_file("../examples/example2.evo");
}