#ifndef HASH_ITERS_H
#define HASH_ITERS_H

#include <cstddef>
#include <utility>
#include "Vector.h"

namespace nepochatova {
  template<class Key, class Value, class Hash, class Equal>
  class HashTable;

  template<class Key, class Value, class Hash, class Equal>
  class HashIter
  {
    friend class HashTable<Key, Value, Hash, Equal>;

  public:
    HashIter();

    HashIter(
      HashTable<Key, Value, Hash, Equal> *table,
      size_t bucket_index,
      typename Vector<std::pair<Key, Value> >::Iterator it
    );

    HashIter &operator++();

    bool operator==(const HashIter &other) const;

    bool operator!=(const HashIter &other) const;

    std::pair<Key, Value> &operator*();

    std::pair<Key, Value> *operator->();

  private:
    HashTable<Key, Value, Hash, Equal> *table_;
    size_t bucket_index_;
    typename Vector<std::pair<Key, Value> >::Iterator chain_it_;
  };


  template<class Key, class Value, class Hash, class Equal>
  class HashConstIter
  {
    friend class HashTable<Key, Value, Hash, Equal>;

  public:
    HashConstIter();

    HashConstIter(
      const HashTable<Key, Value, Hash, Equal> *table,
      size_t bucket_index,
      typename Vector<std::pair<Key, Value> >::CIterator it
    );

    HashConstIter &operator++();

    bool operator==(const HashConstIter &other) const;

    bool operator!=(const HashConstIter &other) const;

    const std::pair<Key, Value> &operator*() const;

    const std::pair<Key, Value> *operator->() const;

  private:
    const HashTable<Key, Value, Hash, Equal> *table_;
    size_t bucket_index_;
    typename Vector<std::pair<Key, Value> >::CIterator chain_it_;
  };
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashIter<Key, Value, Hash, Equal>::HashIter():
  table_(nullptr),
  bucket_index_(0),
  chain_it_()
{}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashIter<Key, Value, Hash, Equal>::HashIter(
  HashTable<Key, Value, Hash, Equal> *table,
  size_t bucket_index,
  typename Vector<std::pair<Key, Value> >::Iterator it):
    table_(table),
    bucket_index_(bucket_index),
    chain_it_(it)
{
  if (!table_) return;

  while (bucket_index_ < table_->data_.getSize() &&
         chain_it_ == table_->data_[bucket_index_].end()) {

    ++bucket_index_;

    while (bucket_index_ < table_->data_.getSize() &&
        table_->data_[bucket_index_].isEmpty()) {
      ++bucket_index_;
    }

    if (bucket_index_ < table_->data_.getSize()) {
      chain_it_ = table_->data_[bucket_index_].begin();
    }
  }
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashIter<Key, Value, Hash, Equal> &
nepochatova::HashIter<Key, Value, Hash, Equal>::operator++()
{
  if (!table_ || bucket_index_ >= table_->data_.getSize()) {
    return *this;
  }

  ++chain_it_;

  while (bucket_index_ < table_->data_.getSize() &&
         chain_it_ == table_->data_[bucket_index_].end()) {
    ++bucket_index_;

    while (bucket_index_ < table_->data_.getSize() &&
        table_->data_[bucket_index_].isEmpty()) {
          ++bucket_index_;
    }

    if (bucket_index_ < table_->data_.getSize()) {
      chain_it_ = table_->data_[bucket_index_].begin();
    }
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashIter<Key, Value, Hash, Equal>::operator==(const HashIter& other) const
{
  if (table_ != other.table_) {
    return false;
  }

  if (bucket_index_ >= table_->data_.getSize() &&
      other.bucket_index_ >= table_->data_.getSize()) {
    return true;
  }

  return bucket_index_ == other.bucket_index_
         && chain_it_ == other.chain_it_;
}

template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashIter<Key, Value, Hash, Equal>::operator!=(const HashIter &other) const
{
  return !(*this == other);
}

template<class Key, class Value, class Hash, class Equal>
std::pair<Key, Value> &
nepochatova::HashIter<Key, Value, Hash, Equal>::operator*()
{
  if (!table_ || bucket_index_ >= table_->data_.getSize())
  {
    throw std::out_of_range("invalid iterator");
  }
  return *chain_it_;
}

template<class Key, class Value, class Hash, class Equal>
std::pair<Key, Value> *
nepochatova::HashIter<Key, Value, Hash, Equal>::operator->()
{
  if (!table_ || bucket_index_ >= table_->data_.getSize())
  {
    throw std::out_of_range("invalid iterator");
  }
  return &(*chain_it_);
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashConstIter<Key, Value, Hash, Equal>::HashConstIter():
  table_(nullptr),
  bucket_index_(0),
  chain_it_() {
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashConstIter<Key, Value, Hash, Equal>::HashConstIter(
  const HashTable<Key, Value, Hash, Equal> *table,
  size_t bucket_index,
  typename Vector<std::pair<Key, Value> >::CIterator it):
    table_(table),
    bucket_index_(bucket_index),
    chain_it_(it)
{
  if (!table_) return;

  while (bucket_index_ < table_->data_.getSize() &&
         chain_it_ == table_->data_[bucket_index_].cend()) {

    ++bucket_index_;

    while (bucket_index_ < table_->data_.getSize() &&
        table_->data_[bucket_index_].isEmpty()) {
      ++bucket_index_;
    }

    if (bucket_index_ < table_->data_.getSize()) {
      chain_it_ = table_->data_[bucket_index_].cbegin();
    }
  }
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashConstIter<Key, Value, Hash, Equal> &
nepochatova::HashConstIter<Key, Value, Hash, Equal>::operator++()
{
  if (!table_ || bucket_index_ >= table_->data_.getSize()) {
    return *this;
  }

  ++chain_it_;

  while (bucket_index_ < table_->data_.getSize() &&
         chain_it_ == table_->data_[bucket_index_].cend()) {

    ++bucket_index_;

    while (bucket_index_ < table_->data_.getSize() &&
        table_->data_[bucket_index_].isEmpty()) {
      ++bucket_index_;
    }

    if (bucket_index_ < table_->data_.getSize()) {
      chain_it_ = table_->data_[bucket_index_].cbegin();
    }
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashConstIter<Key, Value, Hash, Equal>::operator==(const HashConstIter& other) const
{
  if (table_ != other.table_) {
    return false;
  }

  if (bucket_index_ >= table_->data_.getSize() &&
      other.bucket_index_ >= table_->data_.getSize()) {
    return true;
      }

  return bucket_index_ == other.bucket_index_
         && chain_it_ == other.chain_it_;
}
template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashConstIter<Key, Value, Hash, Equal>::operator!=(
  const HashConstIter &other) const
{
  return !(*this == other);
}

template<class Key, class Value, class Hash, class Equal>
const std::pair<Key, Value> &
nepochatova::HashConstIter<Key, Value, Hash, Equal>::operator*() const
{
  if (!table_ || bucket_index_ >= table_->data_.getSize())
  {
    throw std::out_of_range("invalid iterator");
  }
  return *chain_it_;
}

 template<class Key, class Value, class Hash, class Equal>
const std::pair<Key, Value> *
nepochatova::HashConstIter<Key, Value, Hash, Equal>::operator->() const
{
  if (!table_ || bucket_index_ >= table_->data_.getSize())
  {
    throw std::out_of_range("invalid iterator");
  }
  return &(*chain_it_);
}

#endif