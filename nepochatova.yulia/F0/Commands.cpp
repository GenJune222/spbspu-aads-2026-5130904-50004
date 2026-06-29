#include "CmdProcessor.h"
#include "XMLParser.h"

namespace nepochatova {
  CommandProcessor::CommandProcessor(DocumentManager& manager):
      manager_(manager)
  {
    commands_.insert(
        "create",
        &CommandProcessor::createCmd
    );

    commands_.insert(
        "load",
        &CommandProcessor::loadCmd
    );

    commands_.insert(
        "save",
        &CommandProcessor::saveCmd
    );

    commands_.insert(
        "drop",
        &CommandProcessor::dropCmd
    );

    commands_.insert(
        "list",
        &CommandProcessor::listCmd
    );

    commands_.insert(
        "print_tree",
        &CommandProcessor::printTreeCmd
    );

    commands_.insert(
        "print_subtree",
        &CommandProcessor::printSubtreeCmd
    );

    commands_.insert(
        "find",
        &CommandProcessor::findCmd
    );

    commands_.insert(
        "add",
        &CommandProcessor::addCmd
    );

    commands_.insert(
        "delete",
        &CommandProcessor::deleteCmd
    );

    commands_.insert(
        "move",
        &CommandProcessor::moveCmd
    );

    commands_.insert(
        "rename",
        &CommandProcessor::renameCmd
    );

    commands_.insert(
        "set_attr",
        &CommandProcessor::setAttributeCmd
    );

    commands_.insert(
        "remove_attr",
        &CommandProcessor::removeAttributeCmd
    );

    commands_.insert(
        "stats",
        &CommandProcessor::statsCmd
    );

    commands_.insert(
        "insert",
        &CommandProcessor::insertCmd
    );

    commands_.insert(
        "merge",
        &CommandProcessor::mergeCmd
    );

    commands_.insert(
        "split",
        &CommandProcessor::splitCmd
    );
  }

  Vector< std::string > CommandProcessor::split(const std::string &line)
  {
    Vector< std::string > result;

    std::string word;

    for (char ch: line) {
      if (ch == ' ') {
        if (!word.empty()) {
          result.pushBack(word);
          word.clear();
        }
      } else {
        word += ch;
      }
    }

    if (!word.empty()) {
      result.pushBack(word);
    }
    return result;
  }
}
