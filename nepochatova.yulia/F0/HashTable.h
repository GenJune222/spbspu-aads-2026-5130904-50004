#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdexcept>
#include <utility>
#include <string>
#include <xxhash.h>
#include "HashIters.h"

namespace nepochatova {
  template<class Key, class Value, class Hash, class Equal>
  class HashIter;

  template<class Key, class Value, class Hash, class Equal>
  class HashConstIter;

  struct XXHash {
    template<class T>
    size_t operator()(const T& key) const
    {
      return XXH64(
          &key,
          sizeof(T),
          0
      );
    }


    size_t operator()(const std::string& key) const
    {
      return XXH64(
          key.data(),
          key.size(),
          0
      );
    }
  };

  template<class T>
  struct Equal {
    bool operator()(const T &a, const T &b) const
    {
      return a == b;
    }
  };

  template<class Key, class Value, class Hash = XXHash, class Equal = Equal<Key> >
  class HashTable
  {
    friend class HashIter<Key, Value, Hash, Equal>;
    friend class HashConstIter<Key, Value, Hash, Equal>;

  public:
    using HIter = HashIter<Key, Value, Hash, Equal>;
    using HCIter = HashConstIter<Key, Value, Hash, Equal>;

    explicit HashTable(size_t slots = 101);
    ~HashTable() = default;
    HashTable(const HashTable &other);
    HashTable(HashTable &&other) noexcept;

    HashTable &operator=(const HashTable &other);
    HashTable &operator=(HashTable &&other) noexcept;

    void insert(const Key &key, const Value &value);
    Value erase(const Key &key);
    bool contains(const Key &key) const;
    void rehash(size_t new_slots);

    Value &find(const Key &key);
    const Value &find(const Key &key) const;

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

    HIter begin();
    HIter end();
    HCIter begin() const;
    HCIter end() const;
    HCIter cbegin() const;
    HCIter cend() const;

  private:
    using Bucket = Vector<std::pair<Key, Value> >;
    Vector<Bucket> data_;

    size_t size_;
    Hash hasher_;
    Equal equal_;

    size_t getIndex(const Key &key) const;

    static long long findIndexInBucket(const Bucket &bucket, const Key &key, Equal eq);
  };
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable(size_t slots):
  data_(slots > 0 ? slots : 1),
  size_(0)
{}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable &other):
  data_(other.data_.getSize()),
  size_(0), hasher_(other.hasher_),
  equal_(other.equal_)
{
  for (size_t i = 0; i < other.data_.getSize(); ++i) {
    for (const auto &elem: other.data_[i]) {
      insert(elem.first, elem.second);
    }
  }
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable(HashTable &&other) noexcept :
  data_(std::move(other.data_)),
  size_(other.size_),
  hasher_(std::move(other.hasher_)),
  equal_(std::move(other.equal_))
{
  other.size_ = 0;
}


template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal> &
nepochatova::HashTable<Key, Value, Hash, Equal>::operator=(const HashTable &other)
{
  if (this != &other) {
    HashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal> &
nepochatova::HashTable<Key, Value, Hash, Equal>::operator=(HashTable &&other) noexcept
{
  if (this != &other) {
    swap(other);
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::swap(HashTable &other) noexcept
{
  data_.swap(other.data_);
  std::swap(size_, other.size_);
  std::swap(hasher_, other.hasher_);
  std::swap(equal_, other.equal_);
}

template<class Key, class Value, class Hash, class Equal>
size_t nepochatova::HashTable<Key, Value, Hash, Equal>::getIndex(const Key &key) const
{
  return hasher_(key) % data_.getSize();
}

template<class Key, class Value, class Hash, class Equal>
long long nepochatova::HashTable<Key, Value, Hash, Equal>::findIndexInBucket(
  const Bucket &bucket, const Key &key, Equal eq)
{
  for (size_t i = 0; i < bucket.getSize(); ++i) {
    if (eq(bucket[i].first, key)) {
      return static_cast<long long>(i);
    }
  }
  return -1;
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::insert(const Key &key, const Value &value)
{
  size_t idx = getIndex(key);
  Bucket &chain = data_[idx];

  long long existing = findIndexInBucket(chain, key, equal_);
  if (existing >= 0) {
    chain[existing].second = value;
    return;
  }
  try {
    chain.pushBack(std::make_pair(key, value));
    ++size_;
  } catch (const std::bad_alloc &) {
    throw std::overflow_error("Hash table insertion failed: insufficient memory");
  }
}

template<class Key, class Value, class Hash, class Equal>
Value nepochatova::HashTable<Key, Value, Hash, Equal>::erase(const Key &key)
{
  size_t idx = getIndex(key);
  Bucket &chain = data_[idx];

  long long pos = findIndexInBucket(chain, key, equal_);
  if (pos < 0) {
    throw std::out_of_range("Key not found in hash table");
  }

  Value val = chain[pos].second;
  chain.erase(static_cast<size_t>(pos));
  --size_;
  return val;
}

template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashTable<Key, Value, Hash, Equal>::contains(const Key &key) const
{
  size_t idx = getIndex(key);
  return findIndexInBucket(data_[idx], key, equal_) >= 0;
}

template<class Key, class Value, class Hash, class Equal>
Value &nepochatova::HashTable<Key, Value, Hash, Equal>::find(const Key &key)
{
  size_t idx = getIndex(key);
  const Bucket &chain = data_[idx];

  long long pos = findIndexInBucket(chain, key, equal_);
  if (pos < 0) throw std::out_of_range("Key not found");
  return data_[idx][static_cast<size_t>(pos)].second;
}

template<class Key, class Value, class Hash, class Equal>
const Value &nepochatova::HashTable<Key, Value, Hash, Equal>::find(const Key &key) const
{
  size_t idx = getIndex(key);
  const Bucket &chain = data_[idx];

  long long pos = findIndexInBucket(chain, key, equal_);
  if (pos < 0) throw std::out_of_range("Key not found");
  return data_[idx][static_cast<size_t>(pos)].second;
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::rehash(size_t new_slots)
{
  if (new_slots == 0 || new_slots == data_.getSize()) return;

  HashTable<Key, Value, Hash, Equal> new_table(new_slots);

  for (size_t i = 0; i < data_.getSize(); ++i) {
    for (const auto &elem: data_[i]) {
      new_table.insert(elem.first, elem.second);
    }
  }
  swap(new_table);
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::clear() noexcept
{
  for (size_t i = 0; i < data_.getSize(); ++i) {
    data_[i].clear();
  }
  size_ = 0;
}

template<class Key, class Value, class Hash, class Equal>
size_t nepochatova::HashTable<Key, Value, Hash, Equal>::size() const noexcept
{
  return size_;
}

template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashTable<Key, Value, Hash, Equal>::empty() const noexcept
{
  return size_ == 0;
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HIter
nepochatova::HashTable<Key, Value, Hash, Equal>::begin()
{
  for (size_t i = 0; i < data_.getSize(); ++i) {
    if (!data_[i].isEmpty()) {
      return HIter(this, i, data_[i].begin());
    }
  }
  return end();
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HIter
nepochatova::HashTable<Key, Value, Hash, Equal>::end()
{
  return HIter(this, data_.getSize(), typename Vector<std::pair<Key, Value> >::Iterator());
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::begin() const
{
  for (size_t i = 0; i < data_.getSize(); ++i) {

    if (!data_[i].isEmpty()) {
      return HCIter(this, i, data_[i].cbegin());
    }
  }
  return end();
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::end() const
{
  return HCIter(this, data_.getSize(), typename Vector<std::pair<Key, Value> >::CIterator());
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::cbegin() const
{
  return begin();
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::cend() const
{
  return end();
}

#endif
