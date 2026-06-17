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

  Node* DocumentTree::addNode(Node *parent, const std::string &tag)
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

  void DocumentTree::printTree() const
  {
    if (root_) {
      root_->printTree();
    }
  }

  void DocumentTree::printSubtree(const std::string& nodeId) const
  {
    if (!root_) {
      return;
    }

    Vector<Node*> result;

    root_->findById(nodeId, result);

    if (result.isEmpty()) {
      throw std::out_of_range("Node not found");
    }

    result[0]->printTree();
  }

  void DocumentTree::find(const std::string& type, const std::string& value, Vector<Node*>& result)
  {
    if (type == "id") {
      root_->findById(value, result);
    }
    else if (type == "tag") {
      root_->findByTag(value, result);
    }
    else if (type == "class") {
      root_->findByClass(value, result);
    }
  }
}
