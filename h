#!/usr/bin/env python3
import os
import sys
import re

# The H Programming Language Version
__version__ = "0.1.0"

# Error Code Success: 0
# Error Code Unknown Error: 1
# Error Code Unknown Command: 2
# Error Code Invalid Argument: 3
# Error Code File Not Found: 4
# Error Code Variable Not Found: 5
# Error Code Math Error: 6

variables = {}

def bless(text, add_newline=False):
    if text.startswith("&{") and text.endswith("}"):
        varname = text[2:-1]
        if varname in variables:
            print(variables[varname], end='\n' if add_newline else '')
            return 0, None
        else:
            return 5, f"variable {varname} not found"

    elif    (text.startswith('"') and text.endswith('"')) or \
            (text.startswith("'") and text.endswith("'")):
        if len(text) > 1 and text[0] == text[-1]:
            print(text.strip('"').strip("'"), end='\n' if add_newline else '')
            return 0, None
        else:
            return 3, f"argument >>{text}<< needs to be enclosured with either \" \" or \' \'"
    
    elif    (text.startswith("'") and text.endswith('"')) or \
            (text.startswith('"') and text.endswith("'")):
        return 3, f"argument >>{text}<< needs to be enclosured with either \" \" or \' \'"

    else:
        return 3, f"argument >>{text}<< is not a valid string literal or variable reference"

def create_as(args):
    all_args = args.split(' ', 2)

    if len(all_args) != 3:
        return 3, f"create needs at least 3 arguments and {len(all_args)} are given"

    varname = all_args[0]
    keyword_as = all_args[1]
    varvalue = all_args[2]

    if keyword_as != "as":
        return 3, f"invalid create syntax. Expected 'as' at second position, but found '{keyword_as}'"

    if  (varvalue.startswith('"') and varvalue.endswith('"')) or \
        (varvalue.startswith("'") and varvalue.endswith("'")):
        if len(varvalue) > 1 and varvalue[0] == varvalue[-1]:
            variables[varname] = varvalue.strip('"').strip("'")
            return 0, None
        else:
            return 3, f"invalid string literal: {varvalue} string must be enclosured with either \" \" or \' \'"

    try:
        if '.' in varvalue or 'e' in varvalue.lower():
            varvalue = float(varvalue)
            variables[varname] = varvalue
            return 0, None
        else:
            varvalue = int(varvalue)
            variables[varname] = varvalue
            return 0, None
    except ValueError:
        pass

    if varvalue == "true":
        variables[varname] = True
        return 0, None
    elif varvalue == "false":
        variables[varname] = False
        return 0, None
    
    elif varvalue in variables:
        variables[varname] = variables[varvalue]
        return 0, None
    
    else:
        return 3, f"invalid value format for variable {varname} expected quoted string, number, true, false or an existing variable"

def calc(args):
    parts_to = args.rsplit(' to ', 1)
    if len(parts_to) != 2:
        return 3, f"invalid calc syntax. Expected 'to <resultVar>'. Fizbd; '{args}'"
    
    expression_str = parts_to[0].strip()
    result_var_name = parts_to[1].strip()

    processed_expression = expression_str

    sorted_varnames = sorted(variables.keys(), key=len, reverse=True)

    for varname in sorted_varnames:
        varvalue = variables[varname]

        if isinstance(varvalue, (int, float)):
            processed_expression = re.sub(r'\b' + re.escape(varname) + r'\b', str(varvalue), processed_expression)
        elif varname == varvalue:
            pass
        else:
            if re.search(r'\b' + re.escape(varname) + r'\b', processed_expression):
                return 3, f"variable {varname} is not a float or integer"

    try:
        result = eval(processed_expression)
        if not isinstance(result, (int, float)):
            return 1, None
        
    except NameError as e:
        return 5, f"unknown variable in expression: {e}"
    except SyntaxError as e:
        return 3, f"syntax error in calculation expression: {e}"
    except TypeError as e:
        return 3, f"type error in calculation expression: {e}"
    except ZeroDivisionError:
        return 6, "cannot divide by 0"
    except Exception as e:
        return 1, e
    
    variables[result_var_name] = result
    return 0, None

def vanish(args):
    all_args = args.split()
    if len(all_args) < 1:
        return 3, f"vanish needs at least 1 argument and {len(all_args)} are given"
    
    for i, arg in enumerate(all_args):
        if all_args[i] not in variables:
            return 5, f"unknown variable {all_args[i]}"
        variables.pop(all_args[i])
    return 0, None

commands = {
    "bless": bless,
    "create": create_as,
    "calc": calc,
    "vanish": vanish
}

if len(sys.argv) < 2:
    print("H Error: No input file specified.")
    print("Usage: python main.py <your_script.hc>")
    print("Or make your .hc script executable and run it directly: ./your_script.hc")
    exit(1)
elif sys.argv[1] == "--version":
    print(f"The H Programming Language Version: {__version__}")
    exit(0)

file_to_execute = os.path.abspath(sys.argv[1])

if ".hlang" not in file_to_execute:
    print("file isn't .hlang file. cannot execute.")
    exit(1)

with open(file_to_execute, "r", encoding="utf-8") as f:
    lines = f.readlines()
    for line_number, line in enumerate(lines, 1):
        original_line = line.strip()
        stripped_line = original_line.strip('\n')

        if line_number == 1 and stripped_line.startswith("#!"):
                continue

        comment_start_index = -1
        in_string_single = False
        in_string_double = False

        for i, char in enumerate(stripped_line):
            if char == "'" and not in_string_double:
                in_string_single = not in_string_single
            elif char == '"' and not in_string_single:
                in_string_double = not in_string_double

            if char == '~' and not in_string_single and not in_string_double:
                comment_start_index = i
                break

        if comment_start_index != -1:
            stripped_line = stripped_line[:comment_start_index].strip()

        if not stripped_line:
            continue

        parts = stripped_line.split(' ', 1)
        command_name = parts[0]
        arguments = parts[1] if len(parts) > 1 else ""
        newline_count = 0

        while arguments.endswith(" sl"):
            arguments = arguments[:-3]
            newline_count += 1

        if command_name in commands:
            target_function = commands[command_name]
            result_code = 0
            err = None

            if target_function == bless:
                result_code, err = target_function(arguments, add_newline=(newline_count > 0))
            else:
                result_code, err = target_function(arguments)

            if result_code == 0:
                for _ in range(newline_count - 1):
                    print()

            if result_code != 0:
                error_msg = ""
                if result_code == 1:
                    error_msg = f"Errorcode: {result_code}: an unknown error occurred during file execution: {err}"
                elif result_code == 3:
                    error_msg = f"Errorcode: {result_code}: wrong argument: {err}"
                else:
                    error_msg = f"Command {command_name} returned an unexpected error code: {result_code}"

                print(f"H Error on line {line_number}: {error_msg} full line: {original_line}")
                print(f"H: Execution halted due to an error")
                exit(result_code)
            elif result_code == 0 and err:
                print(f"[DEBUG] : {err}")
        else:
            print(f"H Error on line {line_number}: Unknown command {command_name} full line: {original_line}")
            print(f"H: Execution halted due to an unknown command")
            exit(2)