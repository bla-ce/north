#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

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

void cat(const char *path) {
  std::ifstream file{ read_file(path) };

  char c = file.get();

  while(file.good()) {
    std::cout << c;
    c = file.get();
  }
    
  file.close();
}

void interpret(char *argv) {
  if(argv == nullptr) {
    usage();
    std::cerr << "ERROR: no file is provided";
  }

  cat(argv);
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
  const char *subcommand{*argv};

  if (strcmp(*argv, "interpret") == 0) {
    interpret(*++argv);
  } else if (strcmp(*argv, "compile") == 0) {
    assert(false && "TODO: implement compiler");
  } else {
    usage();
    std::cerr << "ERROR: unknown subcommand " << subcommand << '\n';
    return (-1);
  }

  return 0;
}
