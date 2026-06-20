#include "Node.h"

namespace nepochatova {

  Node::Node(const std::string &tag) :
    tag_(tag),
    parent_(nullptr)
  {
    static size_t nextId = 1;
    setAttribute("id",std::to_string(nextId++));
  }

  Node::~Node()
  {
    for (auto child: children_) {
      delete child;
    }
  }

  Node *Node::addChild(const std::string &tag)
  {
    Node *child = new Node(tag);
    child->parent_ = this;
    children_.pushBack(child);

    return child;
  }

  void Node::attachChild(Node* child)
  {
    if (!child) {
      throw std::invalid_argument("null child");
    }
    if (child == this) {
      throw std::invalid_argument("cycle");
    }
    child->parent_ = this;
    children_.pushBack(child);
  }

  void Node::removeChild(Node *node)
  {
    for (size_t i = 0; i < children_.getSize(); ++i) {
      if (children_[i] == node) {
        delete children_[i];
        children_.erase(i);

        return;
      }
    }
  }

  void Node::detachChild(Node *child)
  {
    if (!child) {
      throw std::invalid_argument("null child");
    }

    if (child->parent_ != this)
    {
      throw std::runtime_error("not a child");
    }

    for (size_t i = 0; i < children_.getSize(); ++i) {
      if (children_[i] == child) {
        children_.erase(i);
        child->parent_ = nullptr;
        return;
      }
    }

    throw std::runtime_error(
      "child not found"
    );
  }

  void Node::moveTo(Node *newParent)
  {

    if (!newParent) {
      throw std::invalid_argument("null parent");
    }

    if (parent_ == newParent) {
      return;
    }

    if (newParent == this) {
      throw std::invalid_argument("cycle");
    }

    if (containsInSubtree(newParent)) {
      throw std::invalid_argument("cycle");
    }

    if (parent_) {
      for (size_t i = 0; i < parent_->children_.getSize(); ++i) {
        if (parent_->children_[i] == this) {
          parent_->children_.erase(i);
          break;
        }
      }
    }
    parent_ = newParent;
    newParent->children_.pushBack(this);
  }

  Node *Node::clone() const
  {
    Node *copy = new Node(tag_);

    for (auto it = attributes_.begin(); it != attributes_.end(); ++it){
      copy->setAttribute(it->first,it->second);
    }

    for (auto child: children_) {
      Node *childCopy = child->clone();
      copy->attachChild(childCopy);
    }

    return copy;
  }

  void Node::setAttribute(const std::string &key, const std::string &value)
  {
    attributes_.insert(key, value);
  }

  void Node::removeAttribute(const std::string &key)
  {
    if (attributes_.contains(key)) {
      attributes_.erase(key);
    }
  }

  bool Node::hasAttribute(const std::string &key) const
  {
    return attributes_.contains(key);
  }

  std::string Node::getAttribute(const std::string &key) const
  {
    return attributes_.find(key);
  }

  const HashTable<std::string, std::string>& Node::getAttributes() const
  {
    return attributes_;
  }

  const std::string &Node::getTag() const
  {
    return tag_;
  }

  Node *Node::getParent()
  {
    return parent_;
  }

  const Node *Node::getParent() const
  {
    return parent_;
  }

  Vector<Node *> &Node::getChildren()
  {
    return children_;
  }

  const Vector<Node *> &Node::getChildren() const
  {
    return children_;
  }

  void Node::printTree(size_t depth) const
  {
    for (size_t i = 0; i < depth; ++i) {
      std::cout << "    ";
    }
    std::cout << tag_;

    const auto& attrs = getAttributes();

    for (auto it = attrs.begin(); it != attrs.end(); ++it) {
      std::cout
          << " "
          << it->first
          << "=\""
          << it->second
          << "\"";
    }

    std::cout << "\n";

    for (auto child: children_) {
      child->printTree(depth + 1);
    }
  }

  size_t Node::size() const
  {
    size_t result = 1;

    for (auto child: children_) {
      result += child->size();
    }
    return result;
  }

  size_t Node::depth() const
  {
    size_t maxDepth = 0;

    for (auto child: children_) {
      size_t d = child->depth();

      if (d > maxDepth)
        maxDepth = d;
    }
    return maxDepth + 1;
  }

  void Node::findByTag(const std::string &tag, Vector<Node *> &result)
  {
    if (tag_ == tag) {
      result.pushBack(this);
    }

    for (auto child: children_) {
      child->findByTag(tag, result);
    }
  }

  void Node::findById(const std::string &id, Vector<Node *> &result)
  {
    if (hasAttribute("id") && getAttribute("id") == id) {
      result.pushBack(this);
    }

    for (auto child: children_) {
      child->findById(id, result);
    }
  }

  void Node::findByClass(const std::string &cls, Vector<Node *> &result)
  {
    if (hasAttribute("class") && getAttribute("class") == cls) {
      result.pushBack(this);
    }

    for (auto child: children_) {
      child->findByClass(cls, result);
    }
  }

  void Node::setTag(const std::string &tag)
  {
    tag_ = tag;
  }

  bool Node::containsInSubtree(Node *node) const
  {
    if (this == node)
      return true;

    for (auto child: children_) {
      if (child->containsInSubtree(node))
        return true;
    }

    return false;
  }

  std::string Node::getPath() const
  {
    if (!parent_) {
      return tag_;
    }

    return parent_->getPath() + "/" + tag_;
  }
}
