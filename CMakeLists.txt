cmake_minimum_required(VERSION 3.10)
project(evo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(evo
    src/main.cpp
    src/value.cpp
    src/lexer.cpp
)

# Include directories for headers
target_include_directories(evo PRIVATE "inc")