
# BRUTER


*`UNDER HEAVY DEVELOPMENT`*


# Reserved

- `()` = expression

- `(@@ )` = string delimiter

- `""` = string delimiter

- `''` = string delimiter

- `;` = end of command separator

- `@@` = string delimiter

# Usage

    command;
    command ...;
    command (command (command ...));
    command variable_1 variable_2 ...;
    command name;
    command @@name;
    command 0b00000000;
    command 0x00000000;
    command 0o00000000;
    command 0.000000;
    command 0;

# Types
      
  BRUTER dont have types in the conventional way, bruter has a byte for each variable, this byte define how the variable will be treated by the vm;

    struct 
    {
        unsigned int alloc: 1;
        unsigned int exec: 1;
        unsigned int string: 1;
        unsigned int floating: 1;
        unsigned int other: 4;
    };

  alloc: this bit indicate whether the variable is a pointer or not;
  exec: this bit indicate whether the variable is executable or not;
  string: this bit indicate whether the variable is a string or not;
  floating: this bit indicate whether the variable is a floating point number or not;
  other: this value is not really used, do whatever you want with it;

# Building instructions

  BRUTER include its own build script;

    # clean build for the current system;
    
    ./build.sh

    # for more options, run the build script with the --help flag;
    
    ./build.sh --help
