#include <cassert>
#include <cstring>
#include <iostream>

void usage(const char *program) {
  std::cout << "Usage: " << program << " <SUBCOMMAND>\n";
  std::cout << "SUBCOMMANDS:\n";
  std::cout << "    interpret <program>   interpret file\n";
  std::cout << "    compile   <program>    compile file.\n";
}

int main(int argc, char *argv[]) {
  // get program ()
  const char *program{*argv++};

  if (argc < 2) {
    usage(program);
    std::cerr << "ERROR: not enough arguments\n";
    return -1;
  }

  // get subcommand
  const char *subcommand{*argv};

  if (strcmp(*argv, "interpret") == 0) {
    assert(false && "TODO: implement interpreter");
  } else if (strcmp(*argv, "compile") == 0) {
    assert(false && "TODO: implement compiler");
  } else {
    usage(program);
    std::cerr << "ERROR: unknown subcommand " << subcommand << '\n';
    return (-1);
  }

  return 0;
}
