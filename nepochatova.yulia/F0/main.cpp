#include <iostream>

#include "CmdProcessor.h"
#include "DocumentManager.h"

int main() {
  nepochatova::DocumentManager manager;
  nepochatova::CommandProcessor processor(manager);

  std::string line;

  while (std::getline(std::cin, line)) {
    try {
      processor.execute(line);
    } catch (const std::exception &e) {
      std::cout << "ERROR: "
          << e.what()
          << '\n';
    }
  }

  return 0;
}
