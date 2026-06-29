#include "XMLParser.h"

namespace nepochatova {

  DocumentTree* XMLParser::load(std::istream& in)
  {
    Node* root = loadNode(in);

    if (!root) {
      throw std::runtime_error("Invalid xml file");
    }

    return new DocumentTree(root);
  }


  Node* XMLParser::loadNode(std::istream& in)
  {
    char ch;

    while (in.get(ch)) {
      if (ch == '<') {
        break;
      }
    }

    if (in.eof()) {
      return nullptr;
    }

    if (in.peek() == '/') {
      while (in.get(ch) && ch != '>') {}
      return nullptr;
    }

    std::string tag;

    while (in.get(ch) && !isspace(ch) && ch != '>' && ch != '/') {
      tag += ch;
    }

    if (tag.empty()) {
      return nullptr;
    }

    Node* node = new Node(tag);

    while (ch != '>' && ch != '/') {

      while (isspace(ch)) {
        if (!in.get(ch)) {
          break;
        }
      }

      if (ch == '>' || ch == '/') {
        break;
      }

      std::string key;

      while (ch != '=' && !isspace(ch)) {
        key += ch;

        if (!in.get(ch)) {
          break;
        }
      }

      while (ch != '"') {
        if (!in.get(ch)) {
          break;
        }
      }

      std::string value;

      while (in.get(ch) && ch != '"') {
        value += ch;
      }

      node->setAttribute(key, value);

      in.get(ch);
    }

    if (ch == '/') {
      while (in.get(ch) && ch != '>') {}
      return node;
    }

    while (true) {
      Node* child = loadNode(in);

      if (!child) {
        break;
      }

      node->attachChild(child);
    }

    return node;
  }


  void XMLParser::save(const DocumentTree& tree, std::ostream& out)
  {
    saveNode(tree.getRoot(), out, 0);
  }


  void XMLParser::saveNode(const Node* node, std::ostream& out, size_t depth)
  {
    if (!node) {
      return;
    }

    for (size_t i = 0; i < depth; ++i) {
      out << "\t";
    }

    out << "<" << node->getTag();

    const auto& attributes = node->getAttributes();

    for (auto it = attributes.begin(); it != attributes.end(); ++it) {
      out << " "
          << it->first
          << "=\""
          << it->second
          << "\"";
    }

    if (node->getChildren().isEmpty()) {
      out << "/>\n";
      return;
    }

    out << ">\n";

    for (auto child : node->getChildren()) {
      saveNode(child, out, depth + 1);
    }

    for (size_t i = 0; i < depth; ++i) {
      out << "\t";
    }

    out << "</"
        << node->getTag()
        << ">\n";
  }

}
