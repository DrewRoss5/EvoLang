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
    Value result = machine.run_prog(buffer);
    return result;
}

int main(){
    run_from_file("../examples/squares.evo");
}