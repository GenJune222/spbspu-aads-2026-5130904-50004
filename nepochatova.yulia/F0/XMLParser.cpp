#include "XMLParser.h"

namespace nepochatova {

  DocumentTree *XMLParser::load(const std::string &filename) {
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


  Node *XMLParser::loadNode(std::ifstream &file) {
    char ch;

    while (file.get(ch)) { // ищем начало тега <
      if (ch == '<') {
        break;
      }
    }

    if (file.eof()) {
      return nullptr;
    }

    if (file.peek() == '/') {// если встретили закрывающий тег </tag>
      while (file.get(ch) && ch != '>') {
        //пропускаем имя закрывающего тега
        }
      return nullptr;
    }

    std::string tag;// читаем имя тега

    while (file.get(ch) && !isspace(ch) && ch != '>' && ch != '/') {
      tag += ch;
    }

    if (tag.empty()) {
      return nullptr;
    }

    Node *node = new Node(tag);// читаем атрибуты

    while (ch != '>' && ch != '/') {
      while (isspace(ch)) {
        if (!file.get(ch)) {
          break;
        }
      }

      if (ch == '>' || ch == '/') {
        break;
      }

      std::string key;//читаем атрибут

      while (ch != '=' && !isspace(ch)) { // пропускаем = и "
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

    if (ch == '/') {  // самозакрывающийся тег <img/>
      while (file.get(ch) && ch != '>') {}
      return node;
    }

    while (true) { // обычный тег с детьми
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

    for (size_t i = 0; i < depth; ++i) {// добавляем отступы перед тегом
      file << "\t";
    }
    file << "<" << node->getTag(); // записываем открытие тега

    const auto &attributes = node->getAttributes();// получаем таблицу атрибутов узла

    for (auto it = attributes.begin(); it != attributes.end(); ++it){// перебираем все атрибуты
      file << " " // добавляем атрибут в XML-формате
          << it->first
          << "=\""
          << it->second
          << "\"";
    }

    if (node->getChildren().isEmpty()) { // если детей нет, значит тег пустой
      file << "/>\n";
      return;
    }
    file << ">\n";  // если дети есть, закрываем открывающий тег

    for (auto child: node->getChildren()) { // рекурсивно сохраняем всех детей
      saveNode(child, file, depth + 1);
    }

    for (size_t i = 0; i < depth; ++i) { // после детей нужно сделать отступ
      // чтобы закрывающий тег был на своем уровне
      file << "\t";
    }
    file << "</"
        << node->getTag()
        << ">\n";
  }

}