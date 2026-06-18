#include "XMLParser.h"

namespace nepochatova {

  void XMLParser::save(const DocumentTree &tree, const std::string &filename)
  {
    std::ofstream file(filename);

    if (!file.is_open()) {
      throw std::runtime_error(
        "Cannot open file: " + filename
      );
    }
    saveNode(tree.getRoot(),file,0);
  }


  void XMLParser::saveNode(const Node *node,std::ofstream &file,size_t depth)
  {
    if (!node) {
      return;
    }

    for (size_t i = 0; i < depth; ++i) {
      file << "    ";
    }
    file << "<" << node->getTag();

    const auto &attributes = node->getAttributes();

    for (auto it = attributes.begin(); it != attributes.end(); ++it){
      file << " "
          << it->first
          << "=\""
          << it->second
          << "\"";
    }

    if (node->getChildren().isEmpty()) {
      file << "/>\n";
      return;
    }
    file << ">\n";

    for (auto child: node->getChildren()) {
      saveNode(child, file, depth + 1);
    }

    for (size_t i = 0; i < depth; ++i) {
      file << "    ";
    }
    file << "</"
        << node->getTag()
        << ">\n";
  }

}