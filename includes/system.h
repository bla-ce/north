#ifndef SYSTEM_H
#define SYSTEM_H

#include <cstdlib>

inline auto ASSEMBLE_FUNCTION { "nasm -f elf64 asem.s" };
inline auto COMPILE_FUNCTION { "gcc -o asem asem.o -static -nostdlib -ggdb" };

#endif
