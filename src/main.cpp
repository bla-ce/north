#include <algorithm>
#include <cassert>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "../includes/assembly.h"
#include "../includes/system.h"

class Token {
  public:
    int row{};
    int col{};
    std::string value{};
  private:
};

bool g_run_mode{false};
char *output{};

void usage() {
    std::cout << "Usage: ./north <SUBCOMMAND> [-r]\n";
    std::cout << "SUBCOMMANDS:\n";
    std::cout << "    compile   <file>    compile file.\n";
    std::cout << "OPTIONS:\n";
    std::cout << "    -r                  run program after compilation.\n";
    std::cout << "    -o        <file>    Place the output into file.\n";
}

std::ifstream read_file(const char *path) {
  std::ifstream file;

  file.open(path, std::ifstream::in);

  assert(file.good() && "ERROR: failed to open file");

  return file;
}

std::vector<std::string> split_by_line(const char *path) {
  std::vector<std::string> lines{};
  std::ifstream file{path};

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << path << std::endl;
    return lines;
  }

  std::string line;
  while (std::getline(file, line)) { 
    lines.push_back(line);
  }

  file.close();

  return lines;
}

void tokenize(const std::string line, std::vector<Token> &tokens, int n_line) {
  std::stringstream s(line);
  std::string s_token;

  while (s >> s_token) {
    Token token;
    token.row = n_line;
    token.value = s_token; 

    tokens.push_back(token);
  }
}

void compile(char *argv, std::string output_filename) {
  if (argv == nullptr) {
    usage();
    std::cerr << "ERROR: no file is provided";
  }

  std::ofstream output_file;

  if(output_filename == "") {
    output_filename = "asem";
  }

  output_file.open(output_filename + ".s", std::ofstream::out | std::ofstream::trunc);

  output_file << base_asm();
  output_file << helpers();

  std::vector<std::string> lines{split_by_line(argv)};
  std::vector<Token> tokens{};

  int n_line{};
  for(const auto &line : lines) {
    n_line++;
    tokenize(line, tokens, n_line);
  }

  bool comments { false };
  bool string_literals { false };

  int n_tokens{};

  std::string str {""};
  std::vector<std::string> strings{};
  int count_strings{};

  int n_conditions{1};
  std::stack<int> stack_condition{};

  int n_loops{1};
  std::stack<int> stack_loop{};

  for (const Token &token : tokens) {
    n_tokens++;
    std::string s_token{token.value};

    if(comments) { 
      if(s_token[s_token.length()-1] == ')') {
        comments = false;
      } 

      continue; 
    }

    if(string_literals) { 
      if(s_token[s_token.length()-1] == '"') {
        str += " " + s_token;

        string_literals = false;
        strings.push_back(str);

        output_file << push_string_assembly(count_strings);

        count_strings++;

        str = "";
      } else {
        str += " " + s_token;
      }

      continue; 
    }

    if (s_token == "+") {
      output_file << plus_assembly();
      continue;
    }

    if (s_token == "-") {
      output_file << minus_assembly();
      continue;
    }

    if (s_token == ".") {
      output_file << dump_assembly();
      continue;
    }

    if (s_token == "PRINT") {
      output_file << print_assembly();
      continue;
    }

    if (s_token == "EMIT") {
      output_file << emit_assembly();
      continue;
    }

    if (s_token == "=") {
      output_file << equal_assembly();
      continue;
    }

    if (s_token == "*") {
      output_file << mult_assembly();
      continue;
    }

    if (s_token == "DUP") {
      output_file << dup_assembly();
      continue;
    }

    if (s_token == "2DUP") {
      output_file << two_dup_assembly();
      continue;
    }

    if (s_token == "/") {
      output_file << div_assembly();
      continue;
    }

    if (s_token == "MOD") {
      output_file << mod_assembly();
      continue;
    }

    if (s_token == "/MOD") {
      output_file << div_mod_assembly();
      continue;
    }

    if (s_token == "SWAP") {
      output_file << swap_assembly();
      continue;
    }

    if (s_token == "DROP") {
      output_file << drop_assembly();
      continue;
    }

    if (s_token == "2DROP") { 
      output_file << two_drop_assembly();
      continue;
    }

    if (s_token == "OVER") {
      output_file << over_assembly();
      continue;
    }

    if (s_token == "ROT") {
      output_file << rot_assembly();
      continue;
    }

    if (s_token == "<") {
      output_file << less_assembly();
      continue;
    }

    if (s_token == ">") {
      output_file << greater_assembly();
      continue;
    }

    if (s_token == "0=") {
      output_file << is_zero_assembly();
      continue;
    }

    if (s_token == "0<") {
      output_file << is_negative_assembly();
      continue;
    }

    if (s_token == "0>") {
      output_file << is_positive_assembly();
      continue;
    }    

    if (s_token == "NOT") {
      output_file << not_assembly();
      continue;
    }

    if (s_token == "?DUP") {
      output_file << dup_non_zero_assembly(n_conditions);
      n_conditions++;
      continue;
    }

    if (s_token == "IF") {
      output_file << if_assembly(n_conditions);
      stack_condition.push(n_conditions);
      n_conditions++;
      continue;
    }

    if (s_token == "ELSE") {
      output_file << else_assembly(n_conditions-1);
      continue;
    }

    if (s_token == "THEN") {
      output_file << then_assembly( stack_condition.top() );
      stack_condition.pop();
      continue;
    }

    if (s_token == "AND") {
      output_file << and_assembly();
      continue;
    }

    if (s_token == "OR") {
      output_file << or_assembly();
      continue;
    }

    if (s_token == "1+") {
      output_file << one_plus_assembly();
      continue;
    }

    if (s_token == "1-") {
      output_file << one_minus_assembly();
      continue;
    }
    
    if (s_token == "2+") {
      output_file << two_plus_assembly();
      continue;
    }

    if (s_token == "2-") {
      output_file << two_minus_assembly();
      continue;
    }

    if (s_token == "2*") {
      output_file << two_mult_assembly();
      continue;
    }

    if (s_token == "2/") {
      output_file << two_div_assembly();
      continue;
    }

    if (s_token == "MAX") {
      output_file << max_assembly();
      continue;
    }

    if (s_token == "MIN") {
      output_file << min_assembly();
      continue;
    }

    if (s_token == "*/") {
      output_file << div_mult_assembly();
      continue;
    }

    if (s_token == ">R") {
      output_file << push_to_ret_stack();
      continue;
    }    

    if (s_token == "R>") {
      output_file << push_from_ret_stack(); // Pop value from ret stack and push to param stack
      continue;
    }

    if (s_token == "I") {
      output_file << copy_top_ret_stack(); 
      continue;
    }

    if (s_token == "I'") {
      output_file << copy_second_ret_stack(); 
      continue;
    }

    if (s_token == "J") {
      output_file << copy_third_ret_stack(); 
      continue;
    }

    if (s_token == "DO") {
      output_file << push_to_ret_stack();
      output_file << push_to_ret_stack();

      output_file << do_assembly(n_loops); 
      stack_loop.push(n_loops);
      n_loops++;

      continue;
    }

    if (s_token == "LOOP") {
      output_file << loop_assembly( stack_loop.top() ); 
      stack_loop.pop();
      continue;
    }

    if (s_token == "+LOOP") {
      output_file << plus_loop_assembly( stack_loop.top() ); 
      stack_loop.pop();
      continue;
    }

    if (s_token == "CR") {
      output_file << carriage_return(); 
      continue;
    }

    if (s_token == "SPACES") {
      output_file << print_space(); 
      continue;
    }

    if (s_token == "BEGIN") {
      output_file << begin_assembly(); 
      continue;
    }

    if (s_token == "UNTIL") {
      output_file << until_assembly(); 
      continue;
    }

    if (s_token == "(") {
      if(s_token[s_token.length()-1] == ')') {
        continue;
      }

      comments = true;
      continue;
    }

    if (s_token[0] == '"') {
      //TODO: better way to do that ?
      str += s_token;
      string_literals = true;

      if(s_token[s_token.length()-1] == '"') {
        string_literals = false;
        strings.push_back(s_token);

        output_file << push_string_assembly(count_strings);

        count_strings++;

        str = "";
      }

      continue;
    }
    
    if (std::all_of(s_token.begin(), s_token.end(),
                    [](unsigned char c) { return std::isdigit(c); })) {
      output_file << push_assembly(s_token);
    } else {
      std::cerr << argv << ':' << std::to_string(token.row) << ": error: unknown token " << s_token << '\n';
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

  std::cout << "[CMD] nasm -felf64 " + output_filename + ".s\n";
  assemble_function(output_filename);

  std::cout << "[CMD] ld " +  output_filename + " -o " + output_filename + ".o\n\n";
  compile_function(output_filename);

  auto end { std::chrono::system_clock::now() };
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "[INFO] Compilation finished at " << std::ctime(&end_time) << "\n";

  if(g_run_mode) {
    std::cout << "[INFO] Run program\n";

    std::cout << "[CMD] ./" + output_filename + "\n\n";
    run_function(output_filename);
  }
}

int main(int argc, char *argv[]) {
  std::string output_filename{};

  for (int i{1}; i < argc; ++i) {
    if (argv[i][0] == '-' && argv[i][1]) {
      switch (argv[i][1]) {
      case 'r':
        g_run_mode = true;
        break;
      case 'o':
        output_filename = argv[i+1];
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
    compile(*argv, output_filename);
  } else {
    usage();
    std::cerr << "ERROR: unknown subcommand " << subcommand << '\n';
    return (-1);
  }

  return 0;
}
