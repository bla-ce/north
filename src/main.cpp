#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "../includes/assembly.h"
#include "../includes/system.h"

bool g_run_mode{false};
char *output{};

void usage() {
    std::cout << "Usage: ./north <SUBCOMMAND> [-r]\n";
    std::cout << "SUBCOMMANDS:\n";
    std::cout << "    compile   <file>    compile file.\n";
    std::cout << "OPTIONS:\n";
    std::cout << "    -r                  run program after compilation.\n";
}

std::ifstream read_file(const char *path) {
  std::ifstream file;

  file.open(path, std::ifstream::in);

  assert(file.good() && "ERROR: failed to open file");

  return file;
}

std::vector<std::string> tokenize(const char *path) {
  std::vector<std::string> tokens;

  std::ifstream file{read_file(path)};

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << path << std::endl;
    return tokens;
  }

  std::string token;
  while (file >> token) {
    tokens.push_back(token);
  }

  file.close();

  return tokens;
}

void compile(char *argv) {
  if (argv == nullptr) {
    usage();
    std::cerr << "ERROR: no file is provided";
  }

  std::ofstream output_file;

  output_file.open("asem.s", std::ofstream::out | std::ofstream::trunc);

  output_file << base_asm();
  output_file << helpers();

  std::vector<std::string> tokens{tokenize(argv)};

  bool comments { false };

  for (const auto &token : tokens) {
    if(comments) { 
      if(token[token.length()-1] == ')') {
        comments = false;
      } 

      continue; 
    }

    if (token == "+") {
      output_file << plus_assembly();
      continue;
    }

    if (token == "-") {
      output_file << minus_assembly();
      continue;
    }

    if (token == ".") {
      output_file << dump_assembly();
      continue;
    }

    if (token == "=") {
      output_file << equal_assembly();
      continue;
    }

    if (token == "*") {
      output_file << mult_assembly();
      continue;
    }

    if (token == "DUP") {
      output_file << dup_assembly();
      continue;
    }

    if (token == "/") {
      output_file << div_assembly();
      continue;
    }

    if (token == "MOD") {
      output_file << mod_assembly();
      continue;
    }

    if (token == "/MOD") {
      output_file << div_mod_assembly();
      continue;
    }

    if (token == "SWAP") {
      output_file << swap_assembly();
      continue;
    }

    if (token == "DROP") {
      output_file << drop_assembly();
      continue;
    }

    if (token == "OVER") {
      output_file << over_assembly();
      continue;
    }

    if (token == "ROT") {
      output_file << rot_assembly();
      continue;
    }

    if (token[0] == '(') {
      comments = true;
      continue;
    }

    if (std::all_of(token.begin(), token.end(),
                    [](unsigned char c) { return std::isdigit(c); })) {
      output_file << push_assembly(token);
    } else {
      std::cerr << "ERROR: unknown token " << token;
      exit(-1);
    }
  }

  output_file << exit_assembly(0);

  output_file.close();

  auto start { std::chrono::system_clock::now() };
  std::time_t start_time = std::chrono::system_clock::to_time_t(start);
  std::cout << "\n[INFO] Compilation started at " << std::ctime(&start_time) << "\n";

  std::cout << "[INFO] Generating assembly\n";
  std::cout << "[CMD] nasm -felf64 asem.s\n";
  std::system(ASSEMBLE_FUNCTION);
  std::cout << "[CMD] ld asem -o asem.o\n\n";
  std::system(COMPILE_FUNCTION);

  auto end { std::chrono::system_clock::now() };
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "[INFO] Compilation finished at " << std::ctime(&end_time) << "\n";

  if(g_run_mode) {
    std::cout << "[INFO] Run program\n\n";
    std::system(RUN_FUNCTION);
  }
}

int main(int argc, char *argv[]) {
  for (int i{1}; i < argc; ++i) {
    if (argv[i][0] == '-' && argv[i][1]) {
      switch (argv[i][1]) {
      case 'r':
        g_run_mode = true;
        break;
      default:
        usage();
        std::cerr << "ERROR: wrong option provided\n";
        return -1;
      }
    }
  }

  // get program ()
  const char *program{*argv++};

  if (argc < 2) {
    usage();
    std::cerr << "ERROR: no subcommand is provided\n";
    return -1;
  }

  // get subcommand
  const char *subcommand{*argv++};

  if (strcmp(subcommand, "compile") == 0) {
    compile(*argv);
  } else {
    usage();
    std::cerr << "ERROR: unknown subcommand " << subcommand << '\n';
    return (-1);
  }

  return 0;
}
