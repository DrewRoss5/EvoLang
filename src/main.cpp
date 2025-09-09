#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "../inc/interpreter.hpp"

enum CommandCode{
    HELP,
    RUN,
    SHELL,
};

void print_error(const std::string& message){
    std::cout << "\033[31mError: \033[0m" << message << std::endl;
}

void print_help(){
    std::vector<std::string> commands {
        "Command:",
        "help",
        "run",
        "shell"
    };
    std::vector<std::string> args{
        "Args:",
        "",
        "<file_name>",
        ""
    };
    std::vector<std::string> descriptions{
        "Description:\n",
        "displays this menu",
        "executes a .evo source file",
        "opens an interactive evo shell",
    };
    for (int i = 0; i < 4; i++){
        std::cout << std::setw(12) << std::left << commands[i];
        std::cout << std::setw(15) << std::left << args[i];
        std::cout << descriptions[i] << std::endl;
    }
}

Value run_from_file(std::string file_path){
    if (!file_path.ends_with(".evo"))
        throw std::runtime_error("Invalid source file");
    Interpreter machine;
    std::stringstream buffer;
    std::ifstream prog_file(file_path);
    if (!prog_file.good())
        throw std::runtime_error("Failed to read to program file. Does it exist?");
    buffer << prog_file.rdbuf();
    prog_file.close();
    Value result = machine.run_prog(buffer);
    return result;
}

void run_shell(){
    Interpreter machine;
    std::string input, result;
    while (true){
        std::cout << " > ";
        std::getline(std::cin, input);
        if (input == "exit")
            return;
        try{
            machine.run_expr(input);
            if (!machine.stack_empty()){
                result = machine.stack_top().to_string();
                std::cout << "   " << result << std::endl;
            }
        }
        catch (std::runtime_error e){
            print_error(e.what());
        }
    }
}

int main(int argc, char** argv){
    if (argc < 2){
        print_error("This program takes at least one argument, use \"evo help\" for more info");
        return 1;
    }
    std::unordered_map<std::string, CommandCode> command_map = {{"help", HELP}, {"run", RUN}, {"shell", SHELL}};
    if (!command_map.count(argv[1])){
        print_error("Unrecognized command, use \"evo help\" for more info");
        return 1;
    }
    switch (command_map.at(argv[1])){
        case HELP:
            print_help();
            break;
        case RUN:
            if (argc < 3){
                print_error("No file to run.");
                return 1;
            }
            try{
                run_from_file(argv[2]);
            }
            catch (std::runtime_error e){
                print_error(e.what());
            }
            break;
        case SHELL:
            run_shell(); 
            break;
    }
    return 0;
}