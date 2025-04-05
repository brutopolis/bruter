
# bruter


*`UNDER HEAVY DEVELOPMENT`*


# Reserved

- `()` = expression

- `(@ )` = string delimiter

- `""` = string delimiter

- `''` = string delimiter

- `;` = end of command separator

# Usage

    command;
    command ...;
    command (command (command ...));
    command variable_1 variable_2 ...;
    command name;
    "script whatever";

# Types
      
  bruter essentially has 4 types of data:
  - `any`, a integer, or a native function or anything;
  - `number`, a floating point number, float or double;
  - `string`, a string pointer;
  - `array`, a array, essentially a number;

# Building instructions

  bruter include its own build script;

    # clean build for the current system;
    
    ./build.sh

    # debug build;
    
    ./build.sh --debug

    # cc option:
    # you can also define the path to a compiler;

    ./build.sh --cc path/to/compiler
