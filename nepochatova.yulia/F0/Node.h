#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include "Vector.h"
#include "HashTable.h"

namespace nepochatova {
  class Node
  {
  public:
    explicit Node(const std::string &tag);
    ~Node();
    Node(const Node &) = delete;
    Node &operator=(const Node &) = delete;
    //запрещаю копирование и копирующее присваивание чтобы нельщя было очистить одну и ту же память 2 раза

    Node *addChild(const std::string &tag);
    void removeChild(Node *node);
    void moveTo(Node *newParent);

    void setAttribute(const std::string &key, const std::string &value);
    void removeAttribute(const std::string &key);
    bool hasAttribute(const std::string &key) const;
    std::string getAttribute(const std::string &key) const;

    const std::string &getTag() const;

    Node *getParent();
    const Node *getParent() const;

    Vector<Node *> &getChildren();
    const Vector<Node *> &getChildren() const;

    void printTree(size_t depth = 0) const;

    size_t size() const;
    size_t depth() const;

    void findByTag(const std::string &tag, Vector<Node *> &result);

    void findById(const std::string &id, Vector<Node *> &result);

    void findByClass(const std::string &cls, Vector<Node *> &result);

  private:
    std::string tag_;
    HashTable<std::string, std::string> attributes_;
    Vector<Node *> children_;
    Node *parent_;

    bool containsInSubtree(Node *node) const;
  };
}

#endif