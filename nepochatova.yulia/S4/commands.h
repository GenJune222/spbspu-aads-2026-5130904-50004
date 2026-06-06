#ifndef COMMANDS_H
#define COMMANDS_H

#include "BSTree.h"
#include <string>
#include <fstream>
#include <map>

namespace nepochatova {

  class DictionaryManager {
  public:
    using SingleDict = BSTree<int, std::string>;
    using DictCollection = BSTree<std::string, SingleDict>;

    DictionaryManager();
    void loadFromFile(const std::string& filename);
    void processCmds(std::istream& in, std::ostream& out);

  private:
    DictCollection datasets_;

    using CmdHandler = void (DictionaryManager::*)(std::istream&, std::ostream&);
    std::map<std::string, CmdHandler> commands_;

    void cmdPrint(std::istream& in, std::ostream& out);
    void cmdComplement(std::istream& in, std::ostream& out);
    void cmdIntersect(std::istream& in, std::ostream& out);
    void cmdUnion(std::istream& in, std::ostream& out);

  };

}

#endif