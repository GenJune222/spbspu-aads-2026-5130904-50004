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

  // Правило пяти будет реализовано тут дальше

private:
  BSTNode<Key, Value>* root_;
  BSTNode<Key, Value>* fake_leaf_;
  Compare comp_;

  static BSTNode<Key, Value>* make_fake_leaf() {
    auto* node = new BSTNode<Key, Value>(Key{}, Value{}, nullptr);
    node->left = node;
    node->right = node;
    node->parent = node;

    return node;
  }

  void clear(BSTNode<Key, Value>* node) noexcept {
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
};

}
#endif // NEPOCHATOVA_BSTREE_H