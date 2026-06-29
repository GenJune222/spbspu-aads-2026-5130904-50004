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

    Node *addChild(const std::string &tag);
    void attachChild(Node* child);
    void removeChild(Node* node);
    void detachChild(Node* child);
    void moveTo(Node* newParent);
    Node* clone() const;

    void setAttribute(const std::string &key, const std::string &value);
    void removeAttribute(const std::string &key);
    bool hasAttribute(const std::string &key) const noexcept;
    std::string getAttribute(const std::string &key) const;
    const HashTable<std::string, std::string>& getAttributes() const noexcept;

    const std::string &getTag() const noexcept;

    Node *getParent() noexcept;
    const Node *getParent() const noexcept;

    Vector< Node * > &getChildren() noexcept;
    const Vector< Node * > &getChildren() const noexcept;

    void printTree(size_t depth = 0) const;

    size_t size() const noexcept;
    size_t depth() const noexcept;

    void findByTag(const std::string &tag, Vector< Node * > &result);

    void findById(const std::string &id, Vector< Node * > &result);

    void findByClass(const std::string &cls, Vector< Node * > &result);

    void setTag(const std::string &tag);

    std::string getPath() const;

  private:
    std::string tag_;
    HashTable< std::string, std::string > attributes_;
    Vector< Node *> children_;
    Node *parent_;

    bool containsInSubtree(Node *node) const;
  };
}

#endif
