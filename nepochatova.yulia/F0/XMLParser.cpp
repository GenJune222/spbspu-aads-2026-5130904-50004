#include "XMLParser.h"

namespace nepochatova {

  DocumentTree *XMLParser::load(const std::string &filename)
  {
    std::ifstream file(filename);

    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file: " + filename);
    }
    Node *root = loadNode(file);

    if (!root) {
      throw std::runtime_error("Invalid xml file");
    }
    return new DocumentTree(root);
  }


  Node *XMLParser::loadNode(std::ifstream &file)
  {
    char ch;

    while (file.get(ch)) {
      if (ch == '<') {
        break;
      }
    }

    if (file.eof()) {
      return nullptr;
    }

    if (file.peek() == '/') {
      while (file.get(ch) && ch != '>') {
        }
      return nullptr;
    }

    std::string tag;

    while (file.get(ch) && !isspace(ch) && ch != '>' && ch != '/') {
      tag += ch;
    }

    if (tag.empty()) {
      return nullptr;
    }

    Node *node = new Node(tag);

    while (ch != '>' && ch != '/') {
      while (isspace(ch)) {
        if (!file.get(ch)) {
          break;
        }
      }

      if (ch == '>' || ch == '/') {
        break;
      }

      std::string key;

      while (ch != '=' && !isspace(ch)) {
        key += ch;

        if (!file.get(ch)) {
          break;
        }
      }

      while (ch != '"') {
        if (!file.get(ch)) {
          break;
        }
      }

      std::string value;

      while (file.get(ch) && ch != '"') {
        value += ch;
      }

      node->setAttribute(key, value);
      file.get(ch);
    }

    if (ch == '/') {
      while (file.get(ch) && ch != '>') {}
      return node;
    }

    while (true) {
      Node *child = loadNode(file);

      if (!child) {
        break;
      }
      node->attachChild(child);
    }
    return node;
  }


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
      file << "\t";
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
      file << "\t";
    }
    file << "</"
        << node->getTag()
        << ">\n";
  }

}
