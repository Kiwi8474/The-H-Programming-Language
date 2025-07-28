# The H Programming Language

![The H Programming Language Logo](https://placehold.co/150x150/ADD8E6/000000?text=H)

A simple, interpreted programming language, developed in Python.

## About The H Programming Language

The H Programming Language (short: H) is a minimalist yet powerful programming language designed for clarity and ease of use. It's ideal for scripting and learning the fundamentals of programming languages.

**Current Version:** `0.1.0`

**Current Features:**
* **Variables:** Create and manage variables of various types (strings, numbers, booleans).
* **Output:** Print text and variable values to the console.
* **Calculations:** Perform mathematical operations and store results in variables.
* **Variable Management:** Delete no longer needed variables.
* **Comments:** Supports both full-line and inline comments with the tilde `~` symbol.
* **Newlines:** Control newlines in output with `sl`.
* **Version Query:** Display the interpreter version using `--version`.

## Installation

1. Ensure you have Python 3 installed.
2. Clone this repository:
    ```bash
    git clone [https://github.com/Kiwi8474/The-H-Programming-Language.git](https://github.com/Kiwi8474/The-H-Programming-Language.git)
    cd The-H-Programming-Language
    ```

## Usage

To execute an H script, use the `h` interpreter and provide the path to your `.hlang` file:

```bash
python3 h your_script.hlang
```

To query the interpreter's version:
```bash
python3 h --version
```

**Example:**

Create a file named `hello.hlang` with the following content:
```H
#!/path/to/the/interpreter/script/h
create name as "Alice" ~ A string variable
bless "Hello, " ~ Prints 'Hello, ' to the output without a newline
bless &{name} sl ~ Prints the value of 'name' to the output with a newline
create num1 as 10
create num2 as 5
calc num1 + num2 to result ~ Adds two numbers and saves the result to the given variable. If the variable doesn't exist, then it creates a new one
bless "The result is: " ~ Prints without a newline
bless &{result} sl ~ Prints the result with a newline
```

Execute it:
```bash
python3 h hello.hlang
```

**Expected Output:**
```
Hello, Alice
The result is: 15
```

## File Extension

H scripts use the `.hlang` file extension.

## Error Codes

The H Programming Language uses the following error codes:
* **0:** Success
* **1:** Unknown Error
* **2:** Unknown Command
* **3:** Invalid Argument
* **4:** File Not Found
* **5:** Variable Not Found
* **6:** Math Error

## Future Plans

I am continuously working on expanding The H Programming Language. Planned features include:
* Control structures (if/else statements, loops)
* User input
* Functions
* Graphical interfaces

## Contributing

Contributions are welcome! If you have ideas or suggestions for improvements, feel free to open an issue or create a pull request.

## License

This project is licensed under the MIT License.