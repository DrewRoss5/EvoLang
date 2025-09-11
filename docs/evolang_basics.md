# Introduction to the Evo Language
Evo is a strongly-typed stack-oriented scripting language, that may also be used as a compilation target for other languages. Evo is designed to be both a lightweight scripting language for quick data manipulation and a clean compilation target for other languages. It favors explicit stack operations, making control flow and data handling highly transparent. This document serves as a basic guide on using the language. 

## Getting Started:
In Evo, all instructions are stack operations, meaning that they pop data from the stack to get input(s), and push their output to the top of the stack. 
For instance, to add two numbers, each number is popped from the stack, and the sum is pushed to the stack, so running the following code:
```
push 40
push 2
add
````
Results in a stack of one, with `42` on top. Additionally, one-liner compound commands are supported, with multiple commands being read in reverse order. So, the above commands can also be expressed as:
```
add push 40 push 2
```
This feature can of course be used to perform compound operations, for instance
```
println mul push 2 add push 5 push 5 
```
Will print the result of `2 * (5 + 5)`
## Stack commands:
There are three basic stack commands supported, those commands are:
- push
- pop
- duplicate
### Push:
This instruction pushes a new value to the stack, currently there are four supported data types that can be pushed to the stack, being integers, strings, booleans, and single characters. All of these can be directly pushed on to the stack with the push command, as seen below:
```
push 123
push "Hello, World!"
push TRUE
push 'f'
```
The push command can also be used with the "->" alias, as seen below
```
-> 123
```
While the push command can be explicitly written to clarify intent idiomatically, it is not necessary, as all literals encountered are implicitly pushed, so the above code is equivalent to:
```
'f' TRUE "Hello, World!", 'f'
```
As a single statement. <br><br> This feature is used to make instructions more concise when pushing data as an input. <br><br> Consider our earlier example of
```
println mul push 2 add push 5 push 5 
```
This code can also be expressed as 
```
println mul 2 add 5 5
```

### Pop:
The pop command is straight-forward. it simply removes the top value of the stack and returns nothing. For instance, the following code 
```
push 5
push 8
push 4
pop
mul
```
Will result in a stack of one, with a top value of 40, instead of 20, as 4 was pushed before `mul` was called. 


### Duplicate:
Duplication is done through the `dup` instruction, this simply makes a copy of the current top of the stack, and pushes it. For example
```
push 2
add dup
```
Will result in a value of 4 on top of the stack, as the value `2` is duplicated, so the `add` instruction evaluates `2 + 2`, despite `2` having only been explicitly pushed to the stack once. With the exception of `print` and `println`, all instructions that require inputs pop from the stack, so if you need a particular value for multiple operations, it's important to either duplicate the value, or store it to a variable (more detail on variables can be found further in this guide.)

### Size:
The size command pushes the current number of items on the stack to the top of stack. This count does not include the result of the size operation itself, so for instance if you have a stack containing
```
["hi", TRUE, 5]
```
And run
```
push size
```
The resulting stack will be 
```
["hi", TRUE, 5, 3]
```


## Basic Commands
### Arithmetic Instructions:
Evo has support for addition, subtraction, multiplication, division, and modulo operations (`add`, `sub`, `mul`, `div` and `mod` respectively.) All of these pop the top two values off the stack, and push the corresponding result. As of Evo version 0.1 Only integer-type values can be used for these operations, and as such only integer division is supported, though this is likely to be changed in future versions, with support for floating point values. The first popped value is treated as the right-hand side of the expression, and the second value is treated as the left-hand size. So,
```
push 10
push 5
sub
```
Will result in a stack with a top value of `5`
Whereas 
```
push 5
push 10
sub
```
Will result in a stack with a top of value of `-5`<br>
`add`, `sub`, `mul`, `div` and `mod` can also be accessed with the `+`, `-`, `*`, `/`, and `%` aliases respectively.

### Logical Instructions:
Evo has support for the `and`, `or`, `xor`, and `not` logical operations. These operations can be done on any integral type (ints, bools, and chars), however, performing one of these operations of two values of differing types will result in an error. `and`, `or`, `xor` all function similarly to arithmetic instructions, popping the top two values for inputs, and pushing the result. The result will be of the same type as the operands, for example:
```
xor 42 666
```
Will result in a stack with a top value of `644`. Whereas
```
and TRUE FALSE
```
Will result in a stack with a top value of `FALSE`.<br>
The `not` pops a single value off the top of the stack, and will push `TRUE` if the value is equivalent to zero, so both
```
not FALSE
```
and 
```
not 0
```
Will both result in a stack with a top value of `TRUE`
`and`, `or`, `xor`, `not`, can be accessed using the `&`, `|`, `^`, `!`, aliases respectively.

### Comparison Operations
Comparisons can be performed with the `eq` and `neq` operations (equal and not equal respectively). As with the previous types of operation, these pop the top two values off of the stack for inputs, compare them, and push either `TRUE` or `FALSE` to the stack. Comparing two values of different types is possible, but will always evaluate to false. For example
```
neq 0 FALSE
```
or 
```
neq 'a' "a"
```
Will result in a stack with a top value of `TRUE`.<br>
`eq` and `neq` can be accessed with the `==` and `!=` aliases respectively.<br>
In addition, greater than or less than operations can be used to compare any two integral. The are accessed with `gt` and `lt` respectively, or `gte` and `lte` for "greater than or equal" and "less than or equal" operations. 
`gt`, `lt`, `gte`, `lte` can be accessed with the `>`, `<`, `>=`, `<=` aliases respectively.


## I/O Operations
As of Evo v0.1, there is only user I/O, however file I/O will be implemented in future versions.
### Printing Values
`print` and `println` are both used to print the top value of the stack. These commands can print data of any type, so both
```
push 10
println
```
Will print `10` to the console. And
``` 
println "Hello, Evo"
```
Will print `Hello, Evo` to the console.<br>
The only difference between `print` and `println` is that `println` will print a new line after the output. By default, `print` and `println` only print the top value of the stack without popping it, however using `print_p` or `println_p` will automatically insert a pop instruction after the print instruction.
### Reading Values
At the moment, there are two read instructions, `read` and `readint`, both will read a value from the terminal, and push it to the stack. As the name implies, `readint` reads an integer from the user, whereas `read` will interpret any provided value as a string. For example, running the program 
```
readint 
add 10
println_p
```
And providing an input of "5" will print `15` to the terminal, whereas running 
```
read
add 10
println_p
```
With the same input will result in an error.

## Using Variables
Evo has support for variables, however, these are better thought of a "cache" for stack operations, and not the primary way of storing data. There is one explicit command for variables, and one implicit command.
### Setting variables
To set variable simply use the `set` command like so.
```
set <variable_name>
```
This will set the chosen variable to the top value of the stack, and pop the top value. The same variable can be set multiple times, and variables can change type, so 
```
push 42
set foo
set foo "The meaning of life, the universe and everything"
```
Is a valid program, and will result in an empty stack, with a variable of `foo` set to `"The meaning of life, the universe and everything"`. Variables cannot be directly modified, so to increment a variable called `x`, the following would be used
```
push x
add 1
set x
```

### Getting variables
As shown above, pushing the Value variable to the stack works similarly to pushing a immediate value to the stack, and as such, the explicit use of the `push` instruction is optional. Additionally, pushing a variable to the stack will copy the value, but not provide a reference to the variable, so if the following code is run:
```
set x 9
add 10 x
push x
```
The top value of the stack will still be `9` and not `19` as `x` is still set to`9`.

## Labels and Functions
### Labels 
Labels function similarly to labels in assembly, and are declared as so
```
<label_name>:
```
In general, a label should be immediately be followed by the first instruction it jumps to, with further instructions on the following lines. By style conventions, following lines should be indented. The following is an example of a label that cubes the top value of the stack
```
cube_num: dup
    dup
    mul
    mul
```

### Jumping to Labels
Labels can be called with the `j` or `jif` (pronounced "jump", and "jump if" respectively) commands, followed by the name of the label, so to jump to our previous label unconditionally, we would write
```
j cube_num
```
As mentioned above, the `jif` command can also be used to jump to a label. However, it will pop the top value of the stack does not evaluate to zero, for example, if we only want to jump to the cube label, if the variable `cool_number` is equal to 4, we would write
```
eq cool_number 4
jif cube_num
```
This can be used to implement conditionals, or a loop, as seen in this example
```
push 10             # push 10 onto the stack to start a counter

loop: println       # print the current counter value
sub 1               # subtract 1 from the counter
dup                 # duplicate the counter to be used for comparison
jif loop neq 0      # restart the loop, if the counter is not equal to zero

```
Finally, labels can be called before they are declared, so the following code is valid, and will print `123`
```
push 666                # push an arbitrary value to the stack
j main                  # jump to main program, skipping print_label

print_label: println    # print the top value of the stack
    j exit              # exit the program

main:  
    push 123            # push 123 to the stack
    j print_label

exit:
```

## Functions
As with labels in assembly, labels in Evo do not, by default, return execution the code which called them. However, in Evo, labels can be used as functions as long as two conditions are met

- The label is called with the `call` command
- The label's code eventually reaches a `ret` statement

Calling a function that contains `ret` using `j` or `jif` will result in undefined behavior, as unlike the jump commands `call` pushes the current location to Evo's call stack. Calling a function within a function is also valid, as is recursion, see the `factorial.evo` example in this repository for an example of this 



