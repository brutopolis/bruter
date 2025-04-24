
# BRUTER


*`UNDER HEAVY DEVELOPMENT`*


# Reserved

- `()` = expression

- `{}` = string delimiter

- `;` = end of command separator

# Usage

    command;
    command ...;
    command (command (command ...));
    command variable_1 variable_2 ...;
    command {string};
    command @name value;
    command 0b00000000;
    command 0x00000000;
    command 0o00000000;
    command 0.000000;
    command 0;

# Concept

  BRUTER does not have types, everything is a sequence of 4 or 8 bytes, depending on the system;

  BRUTER does not keep a track of what the user meant to each variable, it just stores the data, it is up to the user to keep track of what is what if needed;

  BRUTER does have only 2 delimiters (expression) and {string};

  NO support escape sequences or anything like that, every byte is exactly what it is;

  BRUTER CANT create a string with unmatched delimiters, what makes it impossible to create a string with a close delimiter unmatched, thats intended, bruter is meant to be as raw as possible;

  BRUTER does not have a garbage collector, it is up to the user to free the memory when needed;

  BRUTER does not have a AST, it is structure is already a tree, so it is not needed;

# Building instructions

  BRUTER include its own build script;

    # clean build for the current system;
    
    ./build.sh

    # for more options, run the build script with the --help flag;
    
    ./build.sh --help
