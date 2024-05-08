#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <string>

// Define system call number for exit
constexpr int SYS_EXIT{60};

// Base assembly code
const std::string BASE_ASM{"section .text\n"
                           "global _start\n\n"
                           "_start:\n"};

inline std::string helpers() { return "  _start:\n"; }

inline std::string push_assembly(std::string value) {
  return "  push " + value + "\n\n";
}

inline std::string plus_assembly() {
  return "  pop rax\n"
         "  pop rbx\n"
         "  add rbx, rax\n"
         "  push rbx\n\n";
}

inline std::string minus_assembly() {
  return "  pop rax\n"
         "  pop rbx\n"
         "  sub rbx, rax\n"
         "  push rbx\n\n";
}

inline std::string mult_assembly() {
  return "  pop rax\n"
         "  pop rbx\n"
         "  imul rbx, rax\n"
         "  push rbx\n\n";
}

inline std::string dump_assembly() {
  return "  ; Initialize register values\n"
         "  pop rax\n"
         "  mov rcx, 10       ; Divisor (for converting to decimal)\n"
         "  mov r8, 0         ; Counter for number of digits\n\n"
         "; Conversion loop\n"
         "  conversion_loop:\n"
         "  xor rdx, rdx      ; Clear remainder\n"
         "  div rcx           ; Divide rax by 10, quotient in rax, remainder "
         "in "
         "  rdx\n\n"
         "  add rdx, '0'      ; Convert remainder to ASCII\n"
         "  push rdx          ; Push ASCII character onto the stack\n"
         "  \n"
         "  inc r8            ; Increment digit counter\n"
         "  test rax, rax     ; Check if quotient is zero\n"
         "  jnz conversion_loop ; If not zero, continue conversion\n\n"
         "  ; Printing loop\n"
         "  print_loop:\n"
         "  mov rax, 1        ; sys_write syscall number\n"
         "  mov rdi, 1        ; stdout file descriptor\n"
         "  mov rdx, 1        ; Number of bytes to write\n"
         "  mov rsi, rsp      ; Address of ASCII character on top of the "
         "stack\n"
         "  add rsp, 8        ; Move stack pointer to the next character\n"
         "  syscall           ; Invoke syscall\n\n"
         "  dec r8            ; Decrement digit counter\n"
         "  jnz print_loop    ; Continue printing until all digits printed\n\n"
         "  mov rax, 60       ; system call number for sys_exit\n"
         "  xor rdi, rdi      ; exit code 0\n"
         "  syscall           ; Invoke syscall\n\n";
}

inline std::string equal_assembly() {
  return "  pop rax\n"
         "  pop rbx\n"
         "  mov rcx, 0\n"
         "  mov rdx, 1\n"
         "  cmp rax, rbx\n"
         "  cmove rcx, rdx\n"
         "  push rcx\n\n";
}

// Function to generate assembly code for exiting
inline std::string exit_assembly(const int EXIT_CODE) {
  return "  mov rax, " + std::to_string(SYS_EXIT) +
         "\n"
         "  mov rdi, " +
         std::to_string(EXIT_CODE) +
         "\n"
         "  syscall\n\n";
}

#endif
