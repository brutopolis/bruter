
# bruter


*`UNDER HEAVY DEVELOPMENT`*


# Table of Contents


- [bruter](#bruter)
- [table of contents](#table-of-contents)
- [Introduction](#introduction)
- [Syntax](#syntax)
- [Usage](#usage)
- [Types](#types)
- [Scope](#scope)
- [Building instructions](#building-instructions)
- [Standard Library](#standard-library)
  - [std.math](#stdmath)
  - [std.type](#stdtype)
  - [std.condition (index-based)](#stdcondition-index-based)
  - [std.hash](#stdhash)
  - [std.loop](#stdloop)
  - [std.mem](#stdmem)
  - [std.list](#stdlist)
  - [std.string](#stdstring)
  - [std.global](#stdglobal)
  - [std](#std)
  - [Dynamic libraries](#dynamic-libraries)



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
    #new "name" value; 
    #new "name" (command ...);
    #new "name" (: value value ...);
    command name;
    "script whatever";
    repeat amount "code";
    while "condition" "code";
    // comment;
    command "string";
    command 'string';
    command (@ string);
    if "condition" "code";
    command (sub: @x @y);

# Types
      
  bruter essentially has 4 types of data:
  - `any`, a integer, or a native function or anything;
  - `number`, a floating point number, float or double;
  - `string`, a string pointer;
  - `list`, a list;

# Building instructions

  bruter include its own build script;

    # clean build for the current system;
    
    ./build.sh

    # debug build;
    
    ./build.sh --debug

    # you can also debug a specific file;

    ./build.sh --debug-file path/to/bruter_file.br

    # wasi build;
    # regular bruter interpreter but using WASI;
    # it also create build/run_bruter.sh which is just a wasmtime command;
    # you can find the WASI executable at build/bruter.wasm;
    
    ./build.sh --wasi path/to/wasi-sdk/clang

    # web build;
    # wasm + js module;
    # you can find bruter.wasm and bruter.js in build/web;

    ./build.sh --web

    # or you may want to define the emcc path direcly;

    ./build.sh --emcc path/to/emcc

    # arduino build;
    # you can find the prepared arduino folder in build/arduino/bruter;

    ./build.sh --ino

    # exec build;
    # this will create a executable file(interpreter + embedded code);
    # you can find the executable file in build/bruter_out;

    ./build.sh --exec path/to/bruter_file.br

    # cc option:
    # you can also define the path to a compiler;

    ./build.sh --cc path/to/compiler

    # you can exclude libs as well;

    ./build.sh --clean

    # you can compile the dynamic libraries from build.sh as well;
    
    ./build.sh --lib path/to/lib.so
    
    note that, dynamic libraries are only supported if not arduino or wasm;

  ## Dynamic libraries

  dynamic libraries functions are actually not part of stdlib itself, it comes from the idea of clean bruter, a bruter withou std that can load dynamic libs, like the std itself, while dl.close is a thing, all dynamic libs are auto-closed at exit(except if you delete the dl.list);

  | Function    | Description                              | Function Signature                           |
  |-------------|------------------------------------------|----------------------------------------------|
  | `dl.open`   | load a library                           | `void function(string path);`                |
  | `dl.close`  | close a library                          | `void function(any libpointer);`             |

  | Variable       | Description              | Type  | Default |
  |----------------|--------------------------|-------|---------|
  | `dl.list`      | List of loaded libraries | List  |         |
