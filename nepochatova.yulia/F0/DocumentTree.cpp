#include "DocumentTree.h"

namespace nepochatova {

  DocumentTree::DocumentTree(const std::string &rootTag)
  {
    root_ = new Node(rootTag);
  }

  DocumentTree::~DocumentTree()
  {
    delete root_;
  }

  Node *DocumentTree::getRoot()
  {
    return root_;
  }

  const Node *DocumentTree::getRoot() const
  {
    return root_;
  }

  Node *DocumentTree::addNode(Node *parent, const std::string &tag)
  {
    if (!parent) {
      throw std::invalid_argument("null parent");
    }
    return parent->addChild(tag);
  }

  void DocumentTree::deleteNode(Node *node)
  {
    if (!node) {
      throw std::invalid_argument("null node");
    }

    if (node == root_) {
      throw std::invalid_argument("cannot delete root");
    }
    node->getParent()->removeChild(node);
  }

  void DocumentTree::moveNode(Node *node, Node *newParent)
  {
    if (!node || !newParent) {
      throw std::invalid_argument("null node");
    }
    node->moveTo(newParent);
  }
}
