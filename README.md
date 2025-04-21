
# BRUTER


*`UNDER HEAVY DEVELOPMENT`*


# Reserved

- `()` = expression

- `(@@ )` = string delimiter

- `""` = string delimiter

- `''` = string delimiter

- `;` = end of command separator

# Usage

    command;
    command ...;
    command (command (command ...));
    command variable_1 variable_2 ...;
    command name;
    command 0b00000000;
    command 0x00000000;
    command 0o00000000;
    command 0.000000;
    command 0;

# Types

  BRUTER dont have types, everything is a sequence of 4 or 8 bytes, depending on the system;

  BRUTER does not keep a track of what the user meant to each variable, it just stores the data, it is up to the user to keep track of what is what if needed;

# Building instructions

  BRUTER include its own build script;

    # clean build for the current system;
    
    ./build.sh

    # for more options, run the build script with the --help flag;
    
    ./build.sh --help
