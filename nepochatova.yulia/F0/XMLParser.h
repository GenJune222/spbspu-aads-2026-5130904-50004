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
    void save(const DocumentTree& tree, const std::string& filename);

  private:
    void saveNode(const Node* node, std::ofstream& file, size_t depth);
  };
}

#endif