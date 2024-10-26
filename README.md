# AAA Language Interpreter

This is an interpreter for a simple programming language. It allows you to define and manipulate variables, perform basic arithmetic operations, conditionally execute code, and print values. 

## Features

1. **Variable Declaration**: Define a new variable.
2. **Assignment**: Assign values to variables.
3. **Increment/Decrement**: Increment or decrement variable values.
4. **Arithmetic Operations**: Multiply, divide, and find the modulus.
5. **Print**: Print strings, numbers, or variable values.
6. **Conditionals**: Use simple `if` statements with conditions.
7. **Comments**: Add comments with `#` or `//`.

## Keywords and Syntax

### Declaration
- Use `adharsh` to declare a new variable with an initial value of 0.
    ```plaintext
    adharsh myVar
    ```

### Assignment
- Use `=` to assign an integer value to a variable.
    ```plaintext
    myVar = 5
    ```

### Print
- Use `anjima` to print a string, integer, or variable value.
    ```plaintext
    anjima "Hello, World!".
    anjima 42.
    anjima myVar.
    ```

### Increment/Decrement
- `variable ++` to increment the variable.
- `variable --` to decrement the variable.
    ```plaintext
    myVar ++
    myVar --
    ```

### Arithmetic Operations
- Multiplication: `variable *= value`
- Division: `variable /= value`
- Modulus: `variable %= value`
    ```plaintext
    myVar *= 2
    myVar /= 2
    myVar %= 3
    ```

### Conditionals
- Use `if (variable == value) { ... }` to execute code only if the condition is met.
    ```plaintext
    if (myVar == 5) {
        anjima "myVar is 5"
    }
    ```

### Exit
- Use `anakha` to exit the program.
    ```plaintext
    anakha
    ```

### Comments
- Add comments using `#` or `//`.
    ```plaintext
    # This is a comment
    // This is also a comment
    ```

