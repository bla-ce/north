#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "../includes/assembly.h"
#include "../includes/system.h"

void usage() {
  std::cout << "Usage: ./north <SUBCOMMAND>\n";
  std::cout << "SUBCOMMANDS:\n";
  std::cout << "    interpret <program>   interpret file\n";
  std::cout << "    compile   <program>    compile file.\n";
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

void interpret(char *argv) {
  if (argv == nullptr) {
    usage();
    std::cerr << "ERROR: no file is provided";
  }
}

void compile(char *argv) {
  if (argv == nullptr) {
    usage();
    std::cerr << "ERROR: no file is provided";
  }

  std::ofstream output;
  output.open("asem.s", std::ofstream::out | std::ofstream::trunc);

  output << base_asm();
  output << helpers();

  std::vector<std::string> tokens{tokenize(argv)};

  for (const auto &token : tokens) {
    if (token == "+") {
      output << plus_assembly();
      continue;
    }

    if (token == "-") {
      output << minus_assembly();
      continue;
    }

    if (token == ".") {
      output << dump_assembly();
      continue;
    }

    if (token == "=") {
      output << equal_assembly();
      continue;
    }

    if (token == "*") {
      output << mult_assembly();
      continue;
    }

    if(token == "DUP") {
      output << dup_assembly();
      continue;
    }

    if (std::all_of(token.begin(), token.end(),
                    [](unsigned char c) { return std::isdigit(c); })) {
      output << push_assembly(token);
    } else {
      std::cerr << "ERROR: unknown token " << token;
      exit(-1);
    }
  }

  output << exit_assembly(0);

  output.close();

  std::system(ASSEMBLE_FUNCTION);
  std::system(COMPILE_FUNCTION);
}

int main(int argc, char *argv[]) {
  // get program ()
  const char *program{*argv++};

  if (argc < 2) {
    usage();
    std::cerr << "ERROR: no subcommand is provided\n";
    return -1;
  }

  // get subcommand
  const char *subcommand{*argv++};

  if (strcmp(subcommand, "interpret") == 0) {
    interpret(*argv);
  } else if (strcmp(subcommand, "compile") == 0) {
    compile(*argv);
  } else {
    usage();
    std::cerr << "ERROR: unknown subcommand " << subcommand << '\n';
    return (-1);
  }

  return 0;
}
