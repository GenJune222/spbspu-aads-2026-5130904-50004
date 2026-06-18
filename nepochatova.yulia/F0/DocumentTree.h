#ifndef DOCUMENT_TREE_H
#define DOCUMENT_TREE_H

#include "Node.h"

namespace nepochatova {
  class DocumentTree
  {
  public:
    explicit DocumentTree(const std::string &rootTag);
    explicit DocumentTree(Node* root);
    ~DocumentTree();
    DocumentTree(const DocumentTree &) = delete;
    DocumentTree &operator=(const DocumentTree &) = delete;

    Node* getRoot();
    const Node *getRoot() const;

    Node* addNode(Node *parent, const std::string &tag);
    void deleteNode(Node *node);

    void moveNode(Node *node, Node *newParent);

    void printTree() const;
    void printSubtree(const std::string& nodeId) const;

    void find(const std::string& type,const std::string& value,Vector<Node*>& result);

  private:
    Node *root_;
  };
}

#endif
