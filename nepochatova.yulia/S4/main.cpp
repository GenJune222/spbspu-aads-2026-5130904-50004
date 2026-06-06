#include <iostream>
#include "commands.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " input_file\n";
    return 1;
  }
  nepochatova::DictionaryManager manager;

  try {
    manager.loadFromFile(argv[1]);
    manager.processCmds(std::cin, std::cout);
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
