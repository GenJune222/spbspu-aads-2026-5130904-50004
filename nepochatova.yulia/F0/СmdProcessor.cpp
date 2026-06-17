#include "CmdProcessor.h"

namespace nepochatova {

  void CommandProcessor::execute(const std::string& commandLine)
  {
    Vector<std::string> args = split(commandLine);

    if (args.isEmpty()) {
      return;
    }

    if (args[0] == "create") {
      createCmd(args);
    }
    else if (args[0] == "drop") {
      dropCmd(args);
    }
    else {
      throw std::invalid_argument(
          "Unknown command"
      );
    }
  }

  void CommandProcessor::createCmd(const Vector<std::string>& args)
  {
    if (args.getSize() != 2) {
      throw std::invalid_argument("Invalid create command");
    }

    manager_.createTree(args[1],"root");// пока просто root без определения названия

    std::cout << "<OK> Tree created\n";
  }

  void CommandProcessor::dropCmd(const Vector<std::string>& args)
  {
    if (args.getSize() != 2) {
      throw std::invalid_argument("Invalid drop command");
    }

    manager_.deleteTree(args[1]);

    std::cout << "<OK> Tree removed\n";
  }
}