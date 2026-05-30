#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdexcept>
#include <utility>
#include "Hash_Iters.h"

namespace nepochatova {
  template<class Key, class Value, class Hash, class Equal>
  class HashIter;

  template<class Key, class Value, class Hash, class Equal>
  class HashConstIter;

  struct XXHash {
    size_t operator()(const std::string &key) const {
      size_t hash = 14695981039346656037ULL;
      for (char c: key) {
        hash ^= static_cast<size_t>(c);
        hash *= 1099511628211ULL;
      }
      return hash;
    }
  };

  template<class T>
  struct Equal {
    bool operator()(const T &a, const T &b) const {
      return a == b;
    }
  };

  template<class Key, class Value, class Hash = XXHash, class Equal = Equal<Key> >
  class HashTable {
    friend class HashIter<Key, Value, Hash, Equal>;
    friend class HashConstIter<Key, Value, Hash, Equal>;

  public:
    using HIter = HashIter<Key, Value, Hash, Equal>;
    using HCIter = HashConstIter<Key, Value, Hash, Equal>;

    explicit HashTable(size_t slots = 101);

    ~HashTable();

    HashTable(const HashTable &other);

    HashTable(HashTable &&other) noexcept;

    HashTable &operator=(const HashTable &other);

    HashTable &operator=(HashTable &&other) noexcept;

    void add(const Key &key, const Value &value);

    Value drop(const Key &key);

    bool has(const Key &key) const noexcept;

    void rehash(size_t new_slots);

    Value &get(const Key &key);

    const Value &get(const Key &key) const;

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

    static ssize_t findIndexInBucket(const Bucket &bucket, const Key &key, Equal eq);
  };
}

template<class Key, class Value, class Hash, class Equal>
size_t nepochatova::HashTable<Key, Value, Hash, Equal>::getIndex(const Key &key) const {
  return hasher_(key) % data_.getSize();
}

template<class Key, class Value, class Hash, class Equal>
ssize_t nepochatova::HashTable<Key, Value, Hash, Equal>::findIndexInBucket(
  const Bucket &bucket, const Key &key, Equal eq
) {
  for (size_t i = 0; i < bucket.getSize(); ++i) {
    if (eq(bucket[i].first, key)) {
      return static_cast<ssize_t>(i);
    }
  }
  return -1;
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable(size_t slots)
  : data_(slots > 0 ? slots : 1), size_(0) {
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::~HashTable() {
  clear();
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::swap(HashTable &other) noexcept {
  data_.swap(other.data_);
  std::swap(size_, other.size_);
  std::swap(hasher_, other.hasher_);
  std::swap(equal_, other.equal_);
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable &other)
  : data_(other.data_.getSize()), size_(0), hasher_(other.hasher_), equal_(other.equal_) {
  for (size_t i = 0; i < other.data_.getSize(); ++i) {
    for (const auto &elem: other.data_[i]) {
      add(elem.first, elem.second);
    }
  }
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable(HashTable &&other) noexcept
  : data_(std::move(other.data_)),
    size_(other.size_),
    hasher_(std::move(other.hasher_)),
    equal_(std::move(other.equal_)) {
  other.size_ = 0;
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable &
nepochatova::HashTable<Key, Value, Hash, Equal>::operator=(const HashTable &other) {
  if (this != &other) {
    HashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
nepochatova::HashTable<Key, Value, Hash, Equal>::HashTable &
nepochatova::HashTable<Key, Value, Hash, Equal>::operator=(HashTable &&other) noexcept {
  if (this != &other) {
    swap(other);
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::add(const Key &key, const Value &value) {
  size_t idx = getIndex(key);
  Bucket &chain = data_[idx];

  size_t existing = findIndexInBucket(chain, key, equal_);
  if (existing >= 0) {
    chain[existing].second = value;
    return;
  }
  try {
    chain.push_back(std::make_pair(key, value));
    ++size_;
  } catch (const std::bad_alloc &) {
    throw std::overflow_error("Hash table insertion failed: insufficient memory");
  }
}

template<class Key, class Value, class Hash, class Equal>
Value nepochatova::HashTable<Key, Value, Hash, Equal>::drop(const Key &key) {
  size_t idx = getIndex(key);
  Bucket &chain = data_[idx];

  ssize_t pos = findIndexInBucket(chain, key, equal_);
  if (pos < 0) {
    throw std::out_of_range("Key not found in hash table");
  }

  Value val = std::move(chain[static_cast<size_t>(pos)].second);
  chain.erase(static_cast<size_t>(pos));
  --size_;
  return val;
}

template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashTable<Key, Value, Hash, Equal>::has(const Key &key) const noexcept {
  size_t idx = getIndex(key);
  return findIndexInBucket(data_[idx], key, equal_) >= 0;
}

template<class Key, class Value, class Hash, class Equal>
Value &nepochatova::HashTable<Key, Value, Hash, Equal>::get(const Key &key) {
  size_t idx = getIndex(key);
  const Bucket &chain = data_[idx];

  ssize_t pos = findIndexInBucket(chain, key, equal_);
  if (pos < 0) throw std::out_of_range("Key not found");
  return data_[idx][static_cast<size_t>(pos)].second;
}

template<class Key, class Value, class Hash, class Equal>
const Value &nepochatova::HashTable<Key, Value, Hash, Equal>::get(const Key &key) const {
  size_t idx = getIndex(key);
  const Bucket &chain = data_[idx];

  ssize_t pos = findIndexInBucket(chain, key, equal_);
  if (pos < 0) throw std::out_of_range("Key not found");
  return data_[idx][static_cast<size_t>(pos)].second;
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::rehash(size_t new_slots) {
  if (new_slots == 0 || new_slots == data_.getSize()) return;

  HashTable<Key, Value, Hash, Equal> new_table(new_slots);

  for (size_t i = 0; i < data_.getSize(); ++i) {
    for (const auto &elem: data_[i]) {
      new_table.add(elem.first, elem.second);
    }
  }
  swap(new_table);
}

template<class Key, class Value, class Hash, class Equal>
void nepochatova::HashTable<Key, Value, Hash, Equal>::clear() noexcept {
  for (size_t i = 0; i < data_.getSize(); ++i) {
    data_[i].clear();
  }
  size_ = 0;
}

template<class Key, class Value, class Hash, class Equal>
size_t nepochatova::HashTable<Key, Value, Hash, Equal>::size() const noexcept {
  return size_;
}

template<class Key, class Value, class Hash, class Equal>
bool nepochatova::HashTable<Key, Value, Hash, Equal>::empty() const noexcept {
  return size_ == 0;
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HIter
nepochatova::HashTable<Key, Value, Hash, Equal>::begin() {
  if (data_.isEmpty()) return end();
  return HIter(this, 0, data_[0].begin());
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HIter
nepochatova::HashTable<Key, Value, Hash, Equal>::end() {
  return HIter(this, data_.getSize(), typename Vector<std::pair<Key, Value> >::Iterator());
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::begin() const {
  if (data_.isEmpty()) return cend();
  return HCIter(this, 0, data_[0].cbegin());
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::end() const {
  return HCIter(this, data_.getSize(), typename Vector<std::pair<Key, Value> >::CIterator());
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::cbegin() const {
  return begin();
}

template<class Key, class Value, class Hash, class Equal>
typename nepochatova::HashTable<Key, Value, Hash, Equal>::HCIter
nepochatova::HashTable<Key, Value, Hash, Equal>::cend() const {
  return end();
}

#endif
