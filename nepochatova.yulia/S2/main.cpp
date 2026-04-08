#include <iostream>
#include <fstream>
#include <string>
#include "stack.h"
#include "operations.h"

int main(int argc, char* argv[])
{
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc > 2) {
    std::cerr << "Error: too many arguments\n";
    return 1;
  }

  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Error: cannot open file '" << argv[1] << "'\n";
      return 1;
    }
    input = &file;
  }

  nepochatova::Stack<long long> results;
  std::string line;

  try {
    while (std::getline(*input, line)) {
      if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
        continue;
      }
      long long res = nepochatova::evaluateExpression(line);
      results.push(res);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  bool first = true;
  while (!results.empty()) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << results.drop();
    first = false;
  }
  std::cout << '\n';

  return 0;
}