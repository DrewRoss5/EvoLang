# ☰ EvoLang ☰
The Evo Programming language is designed to be both a lightweight scripting language for quick data manipulation and a clean compilation target for other languages. It favors explicit stack operations, 
making control flow and data handling highly transparent.

# Project Status
Evo is still early in development, with many features planned for future versions! Here's a brief, non-comprehensive list of planned, and current features
## 📕 Current Features
- Stack Instructions
- Basic arithmetic and logic operatiosn
- Simple command line I/O
- Functions/Loops
- Variables
##  🚀 Planned Features:
- Type Conversions (In Progress)
- In-line conditionals
- Floating point support
- Arrays (In Progress)
- File I/O support
- Support for custom extensions
  
Stay tuned for further developments!

# Getting Started
First, make sure you have **CMake** installed. Then:  
```bash
git clone https://github.com/DrewRoss5/EvoLang.git
cd EvoLang
mkdir build
cd build
cmake ..
make
```
This creates the evo executable. It currently supports two commands:
```
./evo shell        # opens an interactive shell environment
./evo run <file>   # runs a .evo source file
```
# 👋 Example: Hello, Evo!
Here’s a minimal program to get started:
```
# push a string onto the stack, then print and pop it
push "Hello, Evo!"
println_p
```
Some resources to learn more:
- [Evo Language Basic Guide](https://github.com/DrewRoss5/EvoLang/blob/main/docs/evolang_basics.md)
- [Examples](https://github.com/DrewRoss5/EvoLang/tree/main/examples)
