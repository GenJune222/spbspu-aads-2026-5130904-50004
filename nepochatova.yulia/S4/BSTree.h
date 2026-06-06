#ifndef BSTREE_H
#define BSTREE_H

#include <cstddef>
#include <functional>
#include <utility>

namespace nepochatova {
  template<class Key, class Value>
  struct BSTNode {
    Key key;
    Value value;
    BSTNode *left;
    BSTNode *right;
    BSTNode *parent;

    BSTNode(Key k, Value v, BSTNode *p) : key(std::move(k)), value(std::move(v)), left(nullptr), right(nullptr), parent(p) {}
  };

  template<class Key, class Value, class Compare = std::less<Key> >
  class BSTree {
  public:
    BSTree() : root_(nullptr), fake_leaf_(make_fake_leaf()), comp_{} {
      root_ = fake_leaf_;
    }

    ~BSTree() {
      clear();
      delete fake_leaf_;
    }

    BSTree(const BSTree &rhs) : root_(nullptr), fake_leaf_(make_fake_leaf()) , comp_(rhs.comp_) {
      if (rhs.root_ != rhs.fake_leaf_) {
        try {
          root_ = clone_subtree(rhs.root_, nullptr);
        } catch (...) {
          delete fake_leaf_;
          throw;
        }
      } else {
        root_ = fake_leaf_;
      }
    }

    BSTree &operator=(const BSTree &rhs) {
      if (this != &rhs) {
        BSTree tmp(rhs);
        swap(tmp);
      }
      return *this;
    }

    BSTree(BSTree &&rhs) noexcept : root_(rhs.root_), fake_leaf_(rhs.fake_leaf_), comp_(std::move(rhs.comp_)) {
      rhs.root_ = nullptr;
      rhs.fake_leaf_ = nullptr;
    }

    BSTree &operator=(BSTree &&rhs) noexcept {
      if (this != &rhs) {
        clear();
        delete fake_leaf_;

        root_ = rhs.root_;
        fake_leaf_ = rhs.fake_leaf_;
        comp_ = std::move(rhs.comp_);

        rhs.root_ = nullptr;
        rhs.fake_leaf_ = nullptr;
      }
      return *this;
    }

    void swap(BSTree& other) noexcept {
      std::swap(root_, other.root_);
      std::swap(fake_leaf_, other.fake_leaf_);
      std::swap(comp_, other.comp_);
    }

  private:
    BSTNode<Key, Value> *root_;
    BSTNode<Key, Value> *fake_leaf_;
    Compare comp_;

    static BSTNode<Key, Value> *make_fake_leaf() {
      auto *node = new BSTNode<Key, Value>(Key{}, Value{}, nullptr);
      node->left = node;
      node->right = node;
      node->parent = node;

      return node;
    }

    void clear(BSTNode<Key, Value> *node) noexcept {
      if (node == fake_leaf_) {
        return;
      }
      clear(node->left);
      clear(node->right);
      delete node;
    }

    void clear() noexcept {
      clear(root_);
      root_ = fake_leaf_;
    }

    BSTNode<Key, Value>* clone_subtree(BSTNode<Key, Value> *src, BSTNode<Key, Value> *parent) {
      if (src == fake_leaf_) {
        return fake_leaf_;
      }
      auto *new_node = new BSTNode<Key, Value>(src->key, src->value, parent);
      new_node->left = clone_subtree(src->left, new_node);
      new_node->right = clone_subtree(src->right, new_node);
      return new_node;
    }
  };
}
#endif // NEPOCHATOVA_BSTREE_H
