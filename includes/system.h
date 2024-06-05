#ifndef SYSTEM_H
#define SYSTEM_H

#include <cstdlib>
#include <iostream>
#include <string>

inline void assemble_function(const std::string &output_filename) { 
  std::string command { "nasm -felf64 " + output_filename + ".s"};
  std::system(command.c_str());
}

inline void compile_function(const std::string &output_filename) { 
  std::string command {"ld -o " + output_filename + " " + output_filename + ".o"};
  std::system(command.c_str());
}

inline void run_function(const std::string &output_filename) {
  std::string command {"./" + output_filename};
  std::system(command.c_str());
}

#endif
