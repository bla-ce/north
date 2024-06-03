#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
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
  bool string_literals { false };

  std::string str {""};
  std::vector<std::string> strings{};
  int count_strings{};

  int n_conditions{1};
  std::stack<int> stack_condition{};

  int n_loops{1};
  std::stack<int> stack_loop{};

  for (const std::string &token : tokens) {
    if(comments) { 
      if(token[token.length()-1] == ')') {
        comments = false;
      } 

      continue; 
    }

    if(string_literals) { 
      if(token[token.length()-1] == '"') {
        str += " " + token;

        string_literals = false;
        strings.push_back(str);

        output_file << push_string_assembly(count_strings);

        count_strings++;

        str = "";
      } else {
        str += " " + token;
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

    if (token == "PRINT") {
      output_file << print_assembly();
      continue;
    }

    if (token == "EMIT") {
      output_file << emit_assembly();
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

    if (token == "2DUP") {
      output_file << two_dup_assembly();
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

    if (token == "2DROP") { 
      output_file << two_drop_assembly();
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

    if (token == "<") {
      output_file << less_assembly();
      continue;
    }

    if (token == ">") {
      output_file << greater_assembly();
      continue;
    }

    if (token == "0=") {
      output_file << is_zero_assembly();
      continue;
    }

    if (token == "0<") {
      output_file << is_negative_assembly();
      continue;
    }

    if (token == "0>") {
      output_file << is_positive_assembly();
      continue;
    }    

    if (token == "NOT") {
      output_file << not_assembly();
      continue;
    }

    if (token == "?DUP") {
      output_file << dup_non_zero_assembly(n_conditions);
      n_conditions++;
      continue;
    }

    if (token == "IF") {
      output_file << if_assembly(n_conditions);
      stack_condition.push(n_conditions);
      n_conditions++;
      continue;
    }

    if (token == "ELSE") {
      output_file << else_assembly(n_conditions-1);
      continue;
    }

    if (token == "THEN") {
      output_file << then_assembly( stack_condition.top() );
      stack_condition.pop();
      continue;
    }

    if (token == "AND") {
      output_file << and_assembly();
      continue;
    }

    if (token == "OR") {
      output_file << or_assembly();
      continue;
    }

    if (token == "1+") {
      output_file << one_plus_assembly();
      continue;
    }

    if (token == "1-") {
      output_file << one_minus_assembly();
      continue;
    }
    
    if (token == "2+") {
      output_file << two_plus_assembly();
      continue;
    }

    if (token == "2-") {
      output_file << two_minus_assembly();
      continue;
    }

    if (token == "2*") {
      output_file << two_mult_assembly();
      continue;
    }

    if (token == "2/") {
      output_file << two_div_assembly();
      continue;
    }

    if (token == "MAX") {
      output_file << max_assembly();
      continue;
    }

    if (token == "MIN") {
      output_file << min_assembly();
      continue;
    }

    if (token == "*/") {
      output_file << div_mult_assembly();
      continue;
    }

    if (token == ">R") {
      output_file << push_to_ret_stack();
      continue;
    }    

    if (token == "R>") {
      output_file << push_from_ret_stack(); // Pop value from ret stack and push to param stack
      continue;
    }

    if (token == "I") {
      output_file << copy_top_ret_stack(); 
      continue;
    }

    if (token == "I'") {
      output_file << copy_second_ret_stack(); 
      continue;
    }

    if (token == "J") {
      output_file << copy_third_ret_stack(); 
      continue;
    }

    if (token == "DO") {
      output_file << push_to_ret_stack();
      output_file << push_to_ret_stack();

      output_file << do_assembly(n_loops); 
      stack_loop.push(n_loops);
      n_loops++;

      continue;
    }

    if (token == "LOOP") {
      output_file << loop_assembly( stack_loop.top() ); 
      stack_loop.pop();
      continue;
    }

    if (token == "+LOOP") {
      output_file << plus_loop_assembly( stack_loop.top() ); 
      stack_loop.pop();
      continue;
    }

    if (token == "CR") {
      output_file << carriage_return(); 
      continue;
    }

    if (token == "SPACES") {
      output_file << print_space(); 
      continue;
    }

    if (token == "(") {
      if(token[token.length()-1] == ')') {
        continue;
      }

      comments = true;
      continue;
    }

    if (token[0] == '"') {
      //TODO: better way to do that ?
      str += token;
      string_literals = true;

      if(token[token.length()-1] == '"') {
        string_literals = false;
        strings.push_back(token);

        output_file << push_string_assembly(count_strings);

        count_strings++;

        str = "";
      }

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

  output_file << print_ok();

  output_file << exit_assembly(0);

  output_file << section_data();

  int count{};
  for(const auto &string : strings) {
    output_file << add_string_assembly(string, count);
    count++;
  }

  output_file << reserve_return_stack();

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
    std::cout << "[INFO] Run program\n";
    std::cout << "[CMD] ./asem\n\n";
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
