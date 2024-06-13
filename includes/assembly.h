#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <cstdlib>
#include <string>

// TODO: Stack underflow

// Define system call number for exit
constexpr int SYS_EXIT{60};

constexpr int RETURN_STACK_CAP{1024};

// Base assembly code
inline std::string base_asm() {
  return  "section .text\n"
          "global _start\n\n";
}

inline std::string section_data() {
  return  "section .data\n"
          "   ok    db \" ok\", 0Ah\n"
          "   lenok equ $-ok\n\n";
}

inline std::string section_bss() {
  return  "section .bss\n"
          "   pool:     resb 1024\n"
          "   pool_ptr: resd 1\n\n";
}

inline std::string reserve_return_stack() {
  return  "   ; ----- Reserve mem for Return Stack ----- ;\n"
          "   ret_stack     times " + std::to_string(RETURN_STACK_CAP) + " dq 0\n"
          "   ret_stack_ptr dq ret_stack + " + std::to_string(RETURN_STACK_CAP) + " * 8\n\n";
}

inline std::string helpers() {
  return  "dump:\n"
          "   push  rbp\n"
          "   mov   rbp, rsp\n"
          "   mov   r8, 0            ; Counter for number of digits\n\n"
          "   mov   rcx, 10          ; Divisor (for converting to decimal)\n\n"
          "conversion_loop:\n"
          "   xor   rdx, rdx         ; Clear remainder\n"
          "   div   rcx              ; Divide rax by 10, quotient in rax, remainder in rdx\n"
          "   add   rdx, '0'         ; Convert remainder to ASCII\n"
          "   push  rdx              ; Push ASCII character onto the stack\n"
          "   inc   r8               ; Increment digit counter\n"
          "   test  rax, rax         ; Check if quotient is zero\n"
          "   jnz   conversion_loop  ; If not zero, continue conversion\n\n"
          "print_loop:\n"
          "   mov   rax, 1           ; sys_write syscall number\n"
          "   mov   rdi, 1           ; stdout file descriptor\n"
          "   mov   rdx, 1           ; Number of bytes to write\n"
          "   mov   rsi, rsp         ; Address of ASCII character on top of the stack\n"
          "   add   rsp, 8           ; Move stack pointer to the next character\n"
          "   syscall                ; Invoke syscall\n"
          "   dec   r8               ; Decrement digit counter\n"
          "   jnz   print_loop       ; Continue printing until all digits printed\n"
          "   leave \n"
          "   ret\n\n"
          "_start:\n\n"
          "   push rbp\n"
          "   mov rbp, rsp\n\n";
}

inline std::string push_assembly(std::string value) {
  return  "   push " + value + "\n\n";
}

inline std::string push_string_assembly(int count) {
  return  "   push str" +  std::to_string(count) + "      ; push string\n"
          "   push lenstr" +  std::to_string(count) + "   ; push length of string\n\n";
}

inline std::string add_string_assembly(std::string str, int count) {
  return  "   str"    + std::to_string(count) + " db "        + str + "\n"
          "   lenstr" + std::to_string(count) + " equ $-str"  + std::to_string(count) +  "\n\n";
}

inline std::string plus_assembly() {
  return  "   ; ----- PLUS instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   add rbx, rax\n"
          "   push rbx\n\n";
}

inline std::string minus_assembly() {
  return  "   ; ----- MINUS instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   sub rbx, rax\n"
          "   push rbx\n\n";
}

inline std::string mult_assembly() {
  return  "   ; ----- MULTIPLY instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   imul rbx, rax\n"
          "   push rbx\n\n";
}

inline std::string div_assembly() {
  return  "   ; ----- DIVIDE instruction ----- ;\n\n"
          "   xor rdx, rdx\n"
          "   xor rax, rax\n"
          "   pop rcx\n"
          "   pop rax\n"
          "   div rcx\n"
          "   push rax\n\n";
}

inline std::string one_plus_assembly() {
  return  "   ; ----- ONE PLUS instruction ----- ;\n\n"
          "   pop rax\n"
          "   inc rax\n"
          "   push rax\n\n";
}

inline std::string one_minus_assembly() {
  return  "   ; ----- ONE MINUS instruction ----- ;\n\n"
          "   pop rax\n"
          "   dec rax\n"
          "   push rax\n\n";
}

inline std::string two_plus_assembly() {
  return  "   ; ----- TWO PLUS instruction ----- ;\n\n"
          "   pop rax\n"
          "   add rax, 2\n"
          "   push rax\n\n";
}

inline std::string two_minus_assembly() {
  return  "   ; ----- TWO MINUS instruction ----- ;\n\n"
          "   pop rax\n"
          "   sub rax, 2\n"
          "   push rax\n\n";
}

inline std::string two_mult_assembly() {
  return  "   ; ----- TWO MULTIPLY instruction ----- ;\n\n"
          "   pop rax\n"
          "   imul rax, 2\n"
          "   push rax\n\n";
}

inline std::string two_div_assembly() {
  return  "   ; ----- TWO DIVIDE instruction ----- ;\n\n"
          "   xor rdx, rdx\n"
          "   xor rax, rax\n"
          "   mov rcx, 2\n"
          "   pop rax\n"
          "   div rcx\n"
          "   push rax\n\n";
}

inline std::string min_assembly() {
  return  "   ; ----- MIN instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   cmp rbx, rax\n"
          "   cmovl rax, rbx\n"
          "   push rax\n\n";
}

inline std::string max_assembly() {
  return  "   ; ----- MAX instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   cmp rbx, rax\n"
          "   cmovg rax, rbx\n"
          "   push rax\n\n";
}

inline std::string mod_assembly() {
  return  "   ; ----- MODULO instruction ----- ;\n\n"
          "   xor rdx, rdx\n"
          "   xor rax, rax\n"
          "   pop rcx\n"
          "   pop rax\n"
          "   div rcx\n"
          "   push rdx\n\n";
}

inline std::string div_mod_assembly() {
  return  "   ; ----- DIVIDE + MODULO instruction ----- ;\n\n"
          "   xor rdx, rdx\n"
          "   xor rax, rax\n"
          "   pop rcx\n"
          "   pop rax\n"
          "   div rcx\n"
          "   push rdx\n"
          "   push rax\n\n";
}

inline std::string dump_assembly() {
  return  "   ; ----- DUMP instruction ----- ;\n\n"
          "   ; Initialize register values\n"
          "   xor rax, rax\n"
          "   pop rax\n"
          "   call dump\n\n"
          "   push 32\n"
          "   mov rsi, rsp\n"
          "   mov rax, 1\n"
          "   mov rdx, 1\n"
          "   mov rdi, 1\n"
          "   syscall\n"
          "   pop rax\n\n";
}

inline std::string print_assembly() {
  return  "   ; ----- PRINT instruction ----- ;\n\n"
          "   pop rdx\n"
          "   pop rsi\n"
          "   mov rax, 1\n"
          "   mov rdi, 1\n"
          "   syscall\n\n";
}

inline std::string emit_assembly() {
  return  "   ; ----- EMIT instruction ----- ;\n\n"
          "   mov rsi, rsp\n"
          "   mov rdi, 1\n"
          "   mov rdx, 1\n"
          "   mov rax, 1\n"
          "   syscall\n\n"
          "   push 32\n"
          "   mov rsi, rsp\n"
          "   mov rax, 1\n"
          "   mov rdx, 1\n"
          "   mov rdi, 1\n"
          "   syscall\n"
          "   pop rax\n\n";
}

inline std::string carriage_return() {
  return  "   ; ----- CR instruction ----- ;\n\n"
          "   push 10\n"
          "   mov rsi, rsp\n"
          "   mov rax, 1\n"
          "   mov rdx, 1\n"
          "   mov rdi, 1\n"
          "   syscall\n"
          "   pop rax\n\n";
}

inline std::string print_space() {
  return  "   ; ----- SPACES instruction ----- ;\n\n"
          "   pop r9       ; number of spaces\n"
          "   print_space:\n"
          "   dec r9\n"
          "   push 32\n"
          "   mov rsi, rsp\n"
          "   mov rax, 1\n"
          "   mov rdx, 1\n"
          "   mov rdi, 1\n"
          "   syscall\n"
          "   cmp r9, 0\n"
          "   jge print_space\n"
          "   pop rax\n\n";
}

inline std::string equal_assembly() {
  return  "   ; ----- EQUAL instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   mov rcx, 0\n"
          "   mov rdx, 1\n"
          "   cmp rbx, rax\n"
          "   cmove rcx, rdx\n"
          "   push rcx\n\n";
}

inline std::string less_assembly() {
  return  "   ; ----- LESS instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   mov rcx, 0\n"
          "   mov rdx, 1\n"
          "   cmp rbx, rax\n"
          "   cmovl rcx, rdx\n"
          "   push rcx\n\n";
}

inline std::string greater_assembly() {
  return  "   ; ----- GREATER instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   mov rcx, 0\n"
          "   mov rdx, 1\n"
          "   cmp rbx, rax\n"
          "   cmovg rcx, rdx\n"
          "   push rcx\n\n";
}

inline std::string is_zero_assembly() {
  return  "   ; ----- IS ZERO instruction ----- ;\n\n"
          "   pop rax\n"
          "   mov rbx, 0\n"
          "   mov rcx, 0\n"
          "   mov rdx, 1\n"
          "   cmp rbx, rax\n"
          "   cmove rcx, rdx\n"
          "   push rcx\n\n";
}

inline std::string dup_non_zero_assembly(int index) {
  return  "   ; ----- DUP IF NOT ZERO instruction ----- ;\n\n"
          "   pop rax\n"
          "   push rax\n"
          "   test rax, rax\n"
          "   jz skip" + std::to_string(index) + "\n"
          "   push rax\n"
          "   skip" + std::to_string(index) + ":\n\n";
}

inline std::string is_positive_assembly() {
  return  "   ; ----- IS POSITIVE instruction ----- ;\n\n"
          "   pop rax\n"
          "   mov rbx, 0\n"
          "   mov rcx, 0\n"
          "   mov rdx, 1\n"
          "   cmp rax, rbx\n"
          "   cmovg rcx, rdx\n"
          "   push rcx\n\n";
}

inline std::string is_negative_assembly() {
  return  "   ; ----- IS NEGATIVE instruction ----- ;\n\n"
          "   pop rax\n"
          "   mov rbx, 0\n"
          "   mov rcx, 0\n"
          "   mov rdx, 1\n"
          "   cmp rax, rbx\n"
          "   cmovl rcx, rdx\n"
          "   push rcx\n\n";
}

inline std::string not_assembly() {
  return  "   ; ----- NOT instruction ----- ;\n\n"
          "   pop rbx\n"
          "   xor rax, rax\n"
          "   test rbx, rbx\n"
          "   sete al\n"
          "   push rax\n\n";
}

inline std::string dup_assembly() {
  return  "   ; ----- DUP instruction ----- ;\n\n"
          "   pop rax\n"
          "   push rax\n"
          "   push rax\n\n";
}

inline std::string two_dup_assembly() {
  return  "   ; ----- 2DUP instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   push rax\n"
          "   push rbx\n\n"
          "   push rax\n"
          "   push rbx\n\n";
}

inline std::string swap_assembly() {
  return  "   ; ----- SWAP instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   push rax\n"
          "   push rbx\n\n";
}

inline std::string drop_assembly() {
  return  "   ; ----- DROP instruction ----- ;\n\n"
          "   pop rax\n"
          "   xor rax, rax\n\n";
}

inline std::string two_drop_assembly() {
  return  "   ; ----- 2DROP instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rax\n"
          "   xor rax, rax\n\n";
}

inline std::string over_assembly() {
  return  "   ; ----- OVER instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   push rbx\n"
          "   push rax\n"
          "   push rbx\n\n";
}

inline std::string rot_assembly() {
  return  "   ; ----- ROT instruction ----- ;\n\n"
          "   pop rax\n"
          "   pop rbx\n"
          "   pop rdi\n"
          "   push rbx\n"
          "   push rax\n"
          "   push rdi\n\n";
}

inline std::string if_assembly(const int index) {
  return  "   ; ----- IF instruction ----- ;\n\n"
          "   pop rax\n"
          "   test rax, rax\n"
          "   jz addr" + std::to_string(index) + "\n\n";
}

inline std::string else_assembly(const int index) {
  return  "   ; ----- ELSE instruction ----- ;\n\n"
          "   jmp addr" + std::to_string(index+1) + "\n\n"
          "   addr" + std::to_string(index) + ":\n\n";
}

inline std::string then_assembly(const int index) {
  return  "   ; ----- THEN instruction ----- ;\n\n"
          "   addr" + std::to_string(index) + ":\n\n";
}

inline std::string and_assembly() {
  return  "   ; ----- AND instruction ----- ;\n\n"
          "   pop rbx\n"
          "   pop rdi\n"
          "   xor rax, rax\n"
          "   and rbx, rdi\n"
          "   sete al\n"
          "   push rax\n\n";
}

inline std::string or_assembly() {
  return  "   ; ----- OR instruction ----- ;\n\n"
          "   pop rbx\n"
          "   pop rdi\n"
          "   xor rax, rax\n"
          "   or rbx, rdi\n"
          "   sete al\n"
          "   push rax\n\n";
}

inline std::string div_mult_assembly() {
  return  "   ; ----- DIV MULT instruction ----- ;\n\n"
          "   pop rbx\n"
          "   pop rax\n"
          "   pop rdi\n"
          "   imul rdi\n"
          "   xor rdx, rdx\n"
          "   div rbx\n"
          "   push rax\n\n";
}

inline std::string push_to_ret_stack() {
  return  "   ; ----- >R instruction ----- ;\n\n"
          "   xor rax, rax\n"
          "   pop rax\n"
          "   mov rbx, [ret_stack_ptr] ; store ptr address into rbx\n"
          "   sub rbx, 8               ; decrement stack ptr\n"
          "   mov [rbx], rax           ; store rax into rbx\n"
          "   mov [ret_stack_ptr], rbx ; update stack ptr value\n\n";
}

inline std::string push_from_ret_stack() {
  return  "   ; ----- R> instruction ----- ;\n\n"
          "   xor rax, rax\n"
          "   mov rbx, [ret_stack_ptr]  ; store ptr address into rbx\n"
          "   mov rax, [rbx]            ; mov ret value into rax\n"
          "   push rax\n"
          "   add rbx, 8                ; increment stack ptr\n"
          "   mov [ret_stack_ptr], rbx\n\n";
}

inline std::string copy_top_ret_stack() {
  return  "   ; ----- I instruction ----- ;\n\n"
          "   xor rax, rax\n"
          "   xor rbx, rbx\n"
          "   mov rbx, [ret_stack_ptr]  ; store ptr address into rbx\n"
          "   mov rax, [rbx]            ; mov ret value into rax\n"
          "   push rax\n\n";
}

inline std::string copy_second_ret_stack() {
  return  "   ; ----- I' instruction ----- ;\n\n"
          "   xor rax, rax\n"
          "   mov rbx, [ret_stack_ptr]    ; store ptr address into rbx\n"
          "   add rbx, 8\n"
          "   mov rax, [rbx]              ; mov ret value into rax\n"
          "   push rax\n\n";
}

inline std::string copy_third_ret_stack() {
  return  "   ; ----- J instruction ----- ;\n\n"
          "   xor rax, rax\n"
          "   mov rbx, [ret_stack_ptr]    ; store ptr address into rbx\n"
          "   add rbx, 16\n"
          "   mov rax, [rbx]              ; mov ret value into rax\n"
          "   push rax\n\n";
}

inline std::string do_assembly(const int index) {
  return  "   ; ----- DO instruction ----- ;\n\n"
          "   do" + std::to_string(index) + ":\n";
}

inline std::string loop_assembly(const int index) {
  return  "   ; ----- LOOP instruction ----- ;\n\n"
          "   mov rbx, [ret_stack_ptr]\n"
          "   mov rdi, [rbx]\n"
          "   add rbx, 8\n"
          "   mov rax, [rbx]\n"
          "   inc rdi\n"
          "   mov [rbx - 8], rdi\n"
          "   cmp rdi, rax\n"
          "   jl do" + std::to_string(index) + "\n\n"
          "   mov rbx, [ret_stack_ptr]  ; store ptr address into rbx\n"
          "   add rbx, 16                ; increment stack ptr\n"
          "   mov [ret_stack_ptr], rbx\n"
          "   end_loop" + std::to_string(index) + ":\n\n";
}


inline std::string leave_assembly(const int index) {
  return  "   ; ----- LEAVE instruction ----- ;\n\n"
          "   jmp end_loop" + std::to_string(index) + "\n\n";
}

inline std::string plus_loop_assembly(const int index) {
  return  "   ; ----- +LOOP instruction ----- ;\n\n"
          "   pop rcx\n"
          "   mov rbx, [ret_stack_ptr]\n"
          "   mov rdi, [rbx]\n"
          "   add rbx, 8\n"
          "   mov rax, [rbx]\n"
          "   add rdi, rcx\n"
          "   mov [rbx - 8], rdi\n"
          "   cmp rdi, rax\n"
          "   jl do" + std::to_string(index) + "\n\n"
          "   mov rbx, [ret_stack_ptr]  ; store ptr address into rbx\n"
          "   add rbx, 16                ; increment stack ptr\n"
          "   mov [ret_stack_ptr], rbx\n\n";

}

inline std::string begin_assembly() {
  return  "   ; ----- BEGIN instruction ----- ;\n"
          "   begin:\n";
}

inline std::string until_assembly() {
  return  "   ; ----- UNTIL instruction ----- ;\n"
          "   pop rax\n"
          "   test rax, rax\n"
          "   jz begin\n\n";
}

inline std::string print_ok() {
  return  "   mov rax, 1\n"
          "   mov rdi, 1\n"
          "   mov rsi, ok\n"
          "   mov rdx, lenok\n"
          "   syscall\n\n";
}

inline std::string definition_assembly(std::string token) {
  return  "   ; ----- New Definition ----- ;\n"
          "   jmp end" + token + "\n"
          "   "     + token + ":\n";
}

inline std::string end_definition_assembly(std::string token) {
  return  "   ; ----- End Definition ----- ;\n"
          "   end" + token + ":\n";
}

inline std::string pool_assembly() {
  return  "   ; ----- POOL Instruction ----- ;\n"
          "   mov rax, pool\n"
          "   push rax\n\n";
}

// TODO: check for bounds
inline std::string allot_assembly() {
  return  "   ; ----- ALLOT Instruction ----- ;\n";
}

inline std::string store_assembly() {
  return  "   ; ----- STORE (!) Instruction ----- ;\n"
          "   xor rbx, rbx\n"
          "   pop rbx       ; value we want to store\n"
          "   pop rax       ; addr\n"
          "   mov [rax], bl ; mov to pool_ptr the value of rbx\n\n";
}

inline std::string load_assembly() {
  return  "   ; ----- LOAD (@) Instruction ----- ;\n"
          "   pop rax\n"
          "   xor rbx, rbx\n"
          "   mov bl, [rax]\n"
          "   push rbx\n\n";
}

inline std::string syscall1_assembly() {
  return  "   ; ----- SYSCALL1 Instruction ----- ;\n"
          "   pop rdi\n"
          "   pop rax\n"
          "   syscall\n\n";
}

inline std::string syscall2_assembly() {
  return  "   ; ----- SYSCALL2 Instruction ----- ;\n"
          "   pop rsi\n"
          "   pop rdi\n"
          "   pop rax\n"
          "   syscall\n\n";
}

inline std::string syscall3_assembly() {
  return  "   ; ----- SYSCALL3 Instruction ----- ;\n"
          "   pop rdx\n"
          "   pop rsi\n"
          "   pop rdi\n"
          "   pop rax\n"
          "   syscall\n\n";
}

// Function to generate assembly code for exiting
inline std::string exit_assembly(const int EXIT_CODE) {
  return  "   mov rax, " + std::to_string(SYS_EXIT) + "\n"
          "   mov rdi, " + std::to_string(EXIT_CODE) + "\n"
          "   syscall\n\n";
}

#endif
