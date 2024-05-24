#ifndef SYSTEM_H
#define SYSTEM_H

#include <cstdlib>

inline auto ASSEMBLE_FUNCTION { "nasm -felf64 asem.s" };
inline auto COMPILE_FUNCTION { "ld -o asem asem.o" };
inline auto RUN_FUNCTION { "./asem" };

#endif
