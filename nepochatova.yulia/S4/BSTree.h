#ifndef BSTREE_H
#define BSTREE_H

#include <cstddef>
#include <functional>
#include <utility>
#include <stdexcept>

namespace nepochatova {

  template<class Key, class Value>
  struct BSTNode {
    Key key;
    Value value;
    BSTNode *left;
    BSTNode *right;
    BSTNode *parent;

    BSTNode(Key k, Value v, BSTNode *p) :
      key(std::move(k)),
      value(std::move(v)),
      left(nullptr),
      right(nullptr),
      parent(p)
    {}
  };

  template<class Key, class Value>
  class BSTConstIterator;
  template<class Key, class Value>
  class BSTIterator;

  template<class Key, class Value, class Compare = std::less<Key> >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator<Key, Value>;
    using iterator = BSTIterator<Key, Value>;

    BSTree();
    ~BSTree();
    BSTree(const BSTree &rhs);
    BSTree &operator=(const BSTree &rhs);
    BSTree(BSTree &&rhs) noexcept;
    BSTree &operator=(BSTree &&rhs) noexcept;
    void swap(BSTree& other) noexcept;

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool empty() const noexcept;
    bool contains(Key k) const;

    void push(Key k, Value v);
    Value get(Key k) const;
    Value drop(Key k);

    size_t height() const;
    size_t height(const_iterator it) const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

  private:
    template<class K, class V> friend class BSTConstIterator;
    template<class K, class V> friend class BSTIterator;

    BSTNode<Key, Value> *fake_leaf_;
    BSTNode<Key, Value> *root_;
    Compare comp_;

    static BSTNode<Key, Value> *make_fake_leaf();

    void clear(BSTNode<Key, Value> *node) noexcept;
    void clear() noexcept;

    BSTNode<Key, Value>* clone_subtree(BSTNode<Key, Value> *src, BSTNode<Key, Value> *parent);
    BSTNode<Key, Value>* find_min(BSTNode<Key, Value>* node) const;
    size_t calc_height(BSTNode<Key, Value>* node) const;
  };


  template<class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree() :
    fake_leaf_(make_fake_leaf()),
    root_(fake_leaf_),
    comp_
  {}
  {}

  template<class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::~BSTree()
  {
    clear();
    delete fake_leaf_;
  }

  template<class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree(const BSTree &rhs) :
    fake_leaf_(make_fake_leaf()),
    root_(nullptr),
    comp_(rhs.comp_)
  {
    if (!rhs.empty()) {
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

  template<class Key, class Value, class Compare>
  BSTree<Key, Value, Compare> &BSTree<Key, Value, Compare>::operator=(const BSTree &rhs)
  {
    if (this != &rhs) {
      BSTree tmp(rhs);
      swap(tmp);
    }
    return *this;
  }

  template<class Key, class Value, class Compare>
  BSTree<Key, Value, Compare>::BSTree(BSTree &&rhs) noexcept :
    fake_leaf_(rhs.fake_leaf_),
    root_(rhs.root_),
    comp_(std::move(rhs.comp_))
  {
    rhs.root_ = nullptr;
    rhs.fake_leaf_ = nullptr;
  }

  template<class Key, class Value, class Compare>
  BSTree<Key, Value, Compare> &BSTree<Key, Value, Compare>::operator=(BSTree &&rhs) noexcept
  {
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

  template<class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::swap(BSTree &other) noexcept {

    std::swap(fake_leaf_, other.fake_leaf_);
    std::swap(root_, other.root_);
    std::swap(comp_, other.comp_);
  }

  template<class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::iterator BSTree<Key, Value, Compare>::begin()
  {
    return iterator(find_min(root_), fake_leaf_, root_);
  }

  template<class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::iterator BSTree<Key, Value, Compare>::end()
  {
    return iterator(fake_leaf_, fake_leaf_, root_);
  }

  template<class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::cbegin() const
  {
    return const_iterator(find_min(root_), fake_leaf_, root_);
  }

  template<class Key, class Value, class Compare>
  typename BSTree<Key, Value, Compare>::const_iterator BSTree<Key, Value, Compare>::cend() const
  {
    return const_iterator(fake_leaf_, fake_leaf_, root_);
  }

  template<class Key, class Value, class Compare>
  BSTNode<Key, Value>* BSTree<Key, Value, Compare>::make_fake_leaf()
  {
    auto *node = new BSTNode<Key, Value>(Key{}, Value{}, nullptr);
    node->left = node;
    node->right = node;
    node->parent = node;
    return node;
  }

  template<class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::clear(BSTNode<Key, Value> *node) noexcept
  {
    if (node == fake_leaf_) {
      return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
  }

  template<class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::clear() noexcept
  {
    clear(root_);
    root_ = fake_leaf_;
  }

  template<class Key, class Value, class Compare>
  BSTNode<Key, Value>* BSTree<Key, Value, Compare>::clone_subtree(BSTNode<Key, Value> *src, BSTNode<Key, Value> *parent)
  {
    if (src == fake_leaf_) {
      return fake_leaf_;
    }
    auto *new_node = new BSTNode<Key, Value>(src->key, src->value, parent);
    new_node->left = clone_subtree(src->left, new_node);
    new_node->right = clone_subtree(src->right, new_node);
    return new_node;
  }

  template<class Key, class Value, class Compare>
  BSTNode<Key, Value>* BSTree<Key, Value, Compare>::find_min(BSTNode<Key, Value>* node) const
  {
    if (node == fake_leaf_) return fake_leaf_;
    while (node->left != fake_leaf_) {
      node = node->left;
    }
    return node;
  }

  template<class Key, class Value, class Compare>
  size_t BSTree<Key, Value, Compare>::calc_height(BSTNode<Key, Value>* node) const
  {
    if (node == fake_leaf_) return 0;
    return 1 + std::max(calc_height(node->left), calc_height(node->right));
  }

  template<class Key, class Value, class Compare>
  bool BSTree<Key, Value, Compare>::empty() const noexcept
  {
    return root_ == fake_leaf_;
  }

  template<class Key, class Value, class Compare>
  bool BSTree<Key, Value, Compare>::contains(Key k) const
  {
    BSTNode<Key, Value> *cur = root_;
    while (cur != fake_leaf_) {
      if (comp_(k, cur->key)) cur = cur->left;
      else if (comp_(cur->key, k)) cur = cur->right;
      else return true;
    }
    return false;
  }

  template<class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::push(Key k, Value v)
  {
    if (root_ == fake_leaf_) {
      auto *new_node = new BSTNode<Key, Value>(std::move(k), std::move(v), nullptr);
      new_node->left = fake_leaf_;
      new_node->right = fake_leaf_;
      root_ = new_node;
      return;
    }

    BSTNode<Key, Value> *cur = root_;
    while (true) {
      if (comp_(k, cur->key)) {
        if (cur->left == fake_leaf_) {
          auto *new_node = new BSTNode<Key, Value>(std::move(k), std::move(v), cur);
          new_node->left = fake_leaf_;
          new_node->right = fake_leaf_;
          cur->left = new_node;
          return;
        }
        cur = cur->left;
      } else if (comp_(cur->key, k)) {
        if (cur->right == fake_leaf_) {
          auto *new_node = new BSTNode<Key, Value>(std::move(k), std::move(v), cur);
          new_node->left = fake_leaf_;
          new_node->right = fake_leaf_;
          cur->right = new_node;
          return;
        }
        cur = cur->right;
      } else {
        cur->value = std::move(v);
        return;
      }
    }
  }

  template<class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::get(Key k) const
  {
    BSTNode<Key, Value> *cur = root_;
    while (cur != fake_leaf_) {
      if (comp_(k, cur->key)) cur = cur->left;
      else if (comp_(cur->key, k)) cur = cur->right;
      else return cur->value;
    }
    throw std::out_of_range("Key not found in BSTree");
  }

  template<class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::drop(Key k)
  {
    BSTNode<Key, Value> *to_remove = root_;
    while (to_remove != fake_leaf_) {
      if (comp_(k, to_remove->key)) to_remove = to_remove->left;
      else if (comp_(to_remove->key, k)) to_remove = to_remove->right;
      else break;
    }

    if (to_remove == fake_leaf_) {
      throw std::out_of_range("Key not found in BSTree");
    }

    Value result = std::move(to_remove->value);

    if (to_remove->right == fake_leaf_) {
      BSTNode<Key, Value> *child = to_remove->left;
      if (to_remove->parent == nullptr) {
        root_ = child;
        if (root_ != fake_leaf_) root_->parent = nullptr;
      } else {
        if (to_remove->parent->left == to_remove) to_remove->parent->left = child;
        else to_remove->parent->right = child;

        if (child != fake_leaf_) child->parent = to_remove->parent;
      }
      delete to_remove;
      return result;
    }

    BSTNode<Key, Value> *successor = to_remove->right;
    while (successor->left != fake_leaf_) {
      successor = successor->left;
    }

    to_remove->key = std::move(successor->key);
    to_remove->value = std::move(successor->value);

    BSTNode<Key, Value> *succ_child = successor->right;
    if (successor->parent == to_remove) {
      to_remove->right = succ_child;
    } else {
      successor->parent->left = succ_child;
    }
    if (succ_child != fake_leaf_) {
      succ_child->parent = successor->parent;
    }

    delete successor;
    return result;
  }

  template<class Key, class Value, class Compare>
  size_t BSTree<Key, Value, Compare>::height() const
  {
    return calc_height(root_);
  }

  template<class Key, class Value, class Compare>
  size_t BSTree<Key, Value, Compare>::height(const_iterator it) const
  {
    return calc_height(it.node_);
  }

  template<class Key, class Value, class Compare>
  auto BSTree<Key, Value, Compare>::rotateLeft(const_iterator it)-> const_iterator
  {
    BSTNode<Key, Value> *child = it.node_;
    BSTNode<Key, Value> *parent = child->parent;

    if (!parent || child == fake_leaf_) {
      throw std::invalid_argument("Cannot rotate root or fake_leaf");
    }

    parent->right = child->left;
    if (child->left != fake_leaf_) {
      child->left->parent = parent;
    }

    child->parent = parent->parent;
    child->left = parent;

    parent->parent = child;
    if (child->parent) {
      if (child->parent->left == parent)
        child->parent->left = child;
      else
        child->parent->right = child;
    } else {
      root_ = child;
    }

    return it;
  }

  template<class Key, class Value, class Compare>
  auto BSTree<Key, Value, Compare>::rotateRight(const_iterator it)-> const_iterator
  {
    BSTNode<Key, Value> *child = it.node_;
    BSTNode<Key, Value> *parent = child->parent;

    if (!parent || child == fake_leaf_) {
      throw std::invalid_argument("Cannot rotate root or fake_leaf");
    }

    parent->left = child->right;
    if (child->right != fake_leaf_) {
      child->right->parent = parent;
    }

    child->parent = parent->parent;
    child->right = parent;

    parent->parent = child;
    if (child->parent) {
      if (child->parent->left == parent)
        child->parent->left = child;
      else
        child->parent->right = child;
    } else {
      root_ = child;
    }

    return it;
  }

  template<class Key, class Value, class Compare>
  auto BSTree<Key, Value, Compare>::rotateLargeLeft(const_iterator it)-> const_iterator
  {
    auto left_child_it = const_iterator(it.node_->left, fake_leaf_, root_);
    rotateRight(left_child_it);
    return rotateLeft(it);
  }

  template<class Key, class Value, class Compare>
  auto BSTree<Key, Value, Compare>::rotateLargeRight(const_iterator it)-> const_iterator
  {
    auto right_child_it = const_iterator(it.node_->right, fake_leaf_, root_);
    rotateLeft(right_child_it);
    return rotateRight(it);
  }
}
#endif
