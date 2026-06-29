#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <istream>
#include <ostream>

#include "DocumentTree.h"

namespace nepochatova
{
  class XMLParser
  {
  public:
    static DocumentTree* load(std::istream& in);
    static void save(const DocumentTree& tree, std::ostream& out);

  private:
    static Node* loadNode(std::istream& in);
    static void saveNode(const Node* node, std::ostream& out, size_t depth);
  };
}

#endif
