#ifndef DOCUMENT_TREE_H
#define DOCUMENT_TREE_H

#include "Node.h"

namespace nepochatova {
  class DocumentTree
  {
  public:
    explicit DocumentTree(const std::string &rootTag);
    ~DocumentTree();
    DocumentTree(const DocumentTree &) = delete;
    DocumentTree &operator=(const DocumentTree &) = delete;

    Node *getRoot();
    const Node *getRoot() const;

    Node *addNode(Node *parent, const std::string &tag);
    void deleteNode(Node *node);

    void moveNode(Node *node, Node *newParent);

  private:
    Node *root_;
  };
}

#endif
