#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <string>

// TODO: Stack underflow

// Define system call number for exit
constexpr int SYS_EXIT{60};

// Base assembly code
inline std::string base_asm() {
  return
  "section .data\n"
  "   ok db \" ok\", 0Ah\n"
  "   lenok equ $-ok\n\n"
  "section .text\n"
  "global _start\n\n";
}

inline std::string helpers() {
  return
  "dump:\n"
  "   push rbp\n"
  "   mov rbp, rsp\n"
  "   mov r8, 0         ; Counter for number of digits\n\n"
  "   mov rcx, 10       ; Divisor (for converting to decimal)\n\n"
  "conversion_loop:\n"
  "   xor rdx, rdx      ; Clear remainder\n"
  "   div rcx           ; Divide rax by 10, quotient in rax, remainder in rdx\n"
  "   add rdx, '0'      ; Convert remainder to ASCII\n"
  "   push rdx          ; Push ASCII character onto the stack\n"
  "   inc r8            ; Increment digit counter\n"
  "   test rax, rax     ; Check if quotient is zero\n"
  "   jnz conversion_loop ; If not zero, continue conversion\n\n"
  "print_loop:\n"
  "   mov rax, 1        ; sys_write syscall number\n"
  "   mov rdi, 1        ; stdout file descriptor\n"
  "   mov rdx, 1        ; Number of bytes to write\n"
  "   mov rsi, rsp      ; Address of ASCII character on top of the stack\n"
  "   add rsp, 8        ; Move stack pointer to the next character\n"
  "   syscall           ; Invoke syscall\n"
  "   dec r8            ; Decrement digit counter\n"
  "   jnz print_loop    ; Continue printing until all digits printed\n"
  "   leave \n"
  "   ret\n\n"
  "_start:\n";
  "   mov [initial_rsp], rsp\n";
}

inline std::string push_assembly(std::string value) {
  return 
  "   push " + value + "\n\n";
}

inline std::string plus_assembly() {
  return 
  "   ; ----- PLUS instruction ----- ;\n\n"
  "   pop rax\n"
  "   pop rbx\n"
  "   add rbx, rax\n"
  "   push rbx\n\n";
}

inline std::string minus_assembly() {
  return 
  "   ; ----- MINUS instruction ----- ;\n\n"
  "   pop rax\n"
  "   pop rbx\n"
  "   sub rbx, rax\n"
  "   push rbx\n\n";
}

inline std::string mult_assembly() {
  return 
  "   ; ----- MULTIPLY instruction ----- ;\n\n"
  "   pop rax\n"
  "   pop rbx\n"
  "   imul rbx, rax\n"
  "   push rbx\n\n";
}

inline std::string dump_assembly() {
  return 
  "   ; ----- DUMP instruction ----- ;\n\n"
  "   ; Initialize register values\n"
  "   pop rax\n"
  "   call dump\n"
  "   mov rsi, ok\n"
  "   mov rdx, lenok\n"
  "   syscall\n\n";
}

inline std::string equal_assembly() {
  return 
  "   ; ----- EQUAL instruction ----- ;\n\n"
  "   pop rax\n"
  "   pop rbx\n"
  "   mov rcx, 0\n"
  "   mov rdx, 1\n"
  "   cmp rax, rbx\n"
  "   cmove rcx, rdx\n"
  "   push rcx\n\n";
}

inline std::string dup_assembly() {
  return
  "   ; ----- DUP instruction ----- ;\n\n"
  "   pop rax\n"
  "   push rax\n"
  "   push rax\n\n";
}

inline std::string swap_assembly() {
  return
  "   ; ----- SWAP instruction ----- ;\n\n"
  "   pop rax\n"
  "   pop rbx\n"
  "   push rax\n"
  "   push rbx\n\n";
}

// Function to generate assembly code for exiting
inline std::string exit_assembly(const int EXIT_CODE) {
  return 
  "   mov rax, " + std::to_string(SYS_EXIT) + "\n"
  "   mov rdi, " + std::to_string(EXIT_CODE) + "\n"
  "   syscall\n\n";
}

#endif
