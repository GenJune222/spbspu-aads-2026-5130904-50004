#ifndef BSTITERS_H
#define BSTITERS_H

#include "BSTree.h"

namespace nepochatova {

  template <class Key, class Value, class Compare> class BSTree;

  template<class Key, class Value>
  class BSTConstIterator {
    template<class K, class V, class C>
    friend class BSTree;

  protected:
    BSTNode<Key, Value> *node_;
    BSTNode<Key, Value> *fake_leaf_;
    BSTNode<Key, Value> *root_;

    BSTConstIterator(BSTNode<Key, Value> *node, BSTNode<Key, Value> *fake_leaf, BSTNode<Key, Value> *root) : node_(node), fake_leaf_(fake_leaf), root_(root) {}

  public:
    BSTConstIterator() = default;

    BSTConstIterator(const BSTConstIterator &) = default;

    BSTConstIterator &operator=(const BSTConstIterator &) = default;

    std::pair<Key, Value> operator*() const {
      return {node_->key, node_->value};
    }

    BSTConstIterator &operator++() {
      if (node_ == fake_leaf_) return *this;

      if (node_->right != fake_leaf_) {
        node_ = node_->right;
        while (node_->left != fake_leaf_) {
          node_ = node_->left;
        }
      } else {
        BSTNode<Key, Value> *parent = node_->parent;
        while (parent != nullptr && parent != fake_leaf_ && parent->right == node_) {
          node_ = parent;
          parent = node_->parent;
        }
        node_ = (parent == nullptr || parent == fake_leaf_) ? fake_leaf_ : parent;
      }
      return *this;
    }

    BSTConstIterator operator++(int) {
      BSTConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    BSTConstIterator &operator--() {
      if (node_ == fake_leaf_) {
        if (root_ == fake_leaf_) return *this;
        node_ = root_;
        while (node_->right != fake_leaf_) {
          node_ = node_->right;
        }
        return *this;
      }

      if (node_->left != fake_leaf_) {
        node_ = node_->left;
        while (node_->right != fake_leaf_) {
          node_ = node_->right;
        }

      } else {
        BSTNode<Key, Value> *parent = node_->parent;
        while (parent != nullptr && parent != fake_leaf_ && parent->left == node_) {
          node_ = parent;
          parent = node_->parent;
        }
        node_ = (parent == nullptr || parent == fake_leaf_) ? fake_leaf_ : parent;
      }
      return *this;
    }

    BSTConstIterator operator--(int) {
      BSTConstIterator tmp = *this;
      --(*this);
      return tmp;
    }

    bool operator==(const BSTConstIterator &other) const {
      return node_ == other.node_;
    }

    bool operator!=(const BSTConstIterator &other) const {
      return node_ != other.node_;
    }
  };

  template<class Key, class Value>
  class BSTIterator : public BSTConstIterator<Key, Value> {
    template<class K, class V, class C>
    friend class BSTree;

    BSTIterator(BSTNode<Key, Value> *node, BSTNode<Key, Value> *fake_leaf, BSTNode<Key, Value> *root) : BSTConstIterator<Key, Value>(node, fake_leaf, root) {}

  public:
    BSTIterator() = default;

    BSTIterator(const BSTIterator &) = default;

    BSTIterator &operator=(const BSTIterator &) = default;

    std::pair<Key, Value> operator*() const {
      return {this->node_->key, this->node_->value};
    }
  };
}
#endif
