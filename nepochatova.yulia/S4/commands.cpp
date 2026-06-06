#include "commands.h"
#include "BSTIters.h"
#include <iostream>
#include <limits>

namespace nepochatova {
  DictionaryManager::DictionaryManager() : commands_{
    {"print", &DictionaryManager::cmdPrint},
    {"complement", &DictionaryManager::cmdComplement},
    {"intersect", &DictionaryManager::cmdIntersect},
    {"union", &DictionaryManager::cmdUnion}
  } {}

  void DictionaryManager::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string dictName;
    while (file >> dictName) {
      SingleDict dataset;
      int key;
      std::string value;

      while (file.peek() != '\n' && file >> key >> value) {
        dataset.push(key, value);
      }

      datasets_.push(dictName, dataset);
      file.clear();
    }
  }

  void DictionaryManager::processCmds(std::istream &in, std::ostream &out) {
    std::string command;

    while (in >> command) {
      auto it = commands_.find(command);

      if (it == commands_.end()) {
        out << "<INVALID COMMAND>\n";
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }

      try {
        (this->*(it->second))(in, out);
      } catch (const std::logic_error &) {
        out << "<INVALID COMMAND>\n";
      } catch (const std::exception &) {
        throw;
      }

      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (!in.eof()) {
      throw std::runtime_error("Bad input stream");
    }
  }


  void DictionaryManager::cmdPrint(std::istream &in, std::ostream &out) {
    std::string name;
    in >> name;

    if (!in || !datasets_.contains(name)) {
      throw std::logic_error("<INVALID COMMAND>");
    }

    const auto &tree = datasets_.get(name);
    if (tree.empty()) {
      out << "<EMPTY>\n";
      return;
    }

    out << name;
    for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
      out << " " << (*it).first << " " << (*it).second;
    }
    out << "\n";
  }

  void DictionaryManager::cmdComplement(std::istream &in, std::ostream &) {
    std::string newName, leftName, rightName;
    in >> newName >> leftName >> rightName;
    if (!in || !datasets_.contains(leftName) || !datasets_.contains(rightName)) {
      throw std::logic_error("<INVALID COMMAND>");
    }

    const auto &src = datasets_.get(leftName);
    const auto &sub = datasets_.get(rightName);

    SingleDict res;
    for (auto it = src.cbegin(); it != src.cend(); ++it) {
      if (!sub.contains((*it).first)) {
        res.push((*it).first, (*it).second);
      }
    }
    datasets_.push(newName, res);
  }

  void DictionaryManager::cmdIntersect(std::istream &in, std::ostream &) {
    std::string newName, leftName, rightName;
    in >> newName >> leftName >> rightName;
    if (!in || !datasets_.contains(leftName) || !datasets_.contains(rightName)) {
      throw std::logic_error("<INVALID COMMAND>");
    }

    const auto &left = datasets_.get(leftName);
    const auto &right = datasets_.get(rightName);

    SingleDict res;
    for (auto it = left.cbegin(); it != left.cend(); ++it) {
      if (right.contains((*it).first)) {
        res.push((*it).first, (*it).second);
      }
    }
    datasets_.push(newName, res);
  }

  void DictionaryManager::cmdUnion(std::istream &in, std::ostream &) {
    std::string newName, leftName, rightName;
    in >> newName >> leftName >> rightName;
    if (!in || !datasets_.contains(leftName) || !datasets_.contains(rightName)) {
      throw std::logic_error("<INVALID COMMAND>");
    }

    const auto &left = datasets_.get(leftName);
    const auto &right = datasets_.get(rightName);

    SingleDict res = left;

    for (auto it = right.cbegin(); it != right.cend(); ++it) {
      if (!left.contains((*it).first)) {
        res.push((*it).first, (*it).second);
      }
    }
    datasets_.push(newName, res);
  }
}