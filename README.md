# Own Implementation of FORTH Programming Language

WARNING: This is not an exact implementation of the Forth Programming Language. Make sure to read the documentation (when ready).

## References
    Forth, Wikipedia: https://en.wikipedia.org/wiki/Forth_(programming_language)
    Forth, Documentation: https://www.forth.com/wp-content/uploads/2018/01/Starting-FORTH.pdf

## Examples

Program Printing Numbers from 1 to 10

``` forth
    0 10 DO 
        I . 
    LOOP
```

Program Printing Multiplication Table from 1 to 10

``` forth
    CR 1 11 DO
        1 11 DO 
        I J * . 
        LOOP CR
    LOOP
```
## Quick Start

Building the Project
To build the project, follow these steps:

Clone the repository:

``` bash
    git clone git@github.com:bla-ce/north.git
```

Navigate to the project directory:

``` bash
    cd north 
```

Compile the source code:

``` bash
    cd build/
    cmake ..
    make
```

Run the North Compiler:

``` bash
    Usage: ./north <SUBCOMMAND> [-r]
    SUBCOMMANDS:
        compile   <file>    compile file.
    OPTIONS:
        -r                  run program after compilation.
```

This implementation may not support all features of the standard Forth language, and some features might be implemented differently. 
Check the documentation for details on supported features.
