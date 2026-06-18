#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <fstream>

#include "DocumentTree.h"

namespace nepochatova
{
  class XMLParser
  {
  public:
    XMLParser() = default;
    static DocumentTree* load(const std::string& filename);
    void save(const DocumentTree& tree, const std::string& filename);

  private:
    static Node* loadNode(std::ifstream& file);
    void saveNode(const Node* node, std::ofstream& file, size_t depth);
  };
}

#endif