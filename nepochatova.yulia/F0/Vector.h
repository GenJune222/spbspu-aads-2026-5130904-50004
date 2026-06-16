#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>
#include <utility>
#include "VecIters.h"

namespace nepochatova {
  template <class T>
  class Vector
  {

   public:
    using Iterator = nepochatova::Iterator<T>;
    using CIterator = nepochatova::CIterator<T>;

    Vector();
    ~Vector();
    explicit Vector(size_t size);
    Vector(size_t size, const T& value);

    Vector(const Vector<T>& rhs);
    Vector(Vector<T>&& rhs) noexcept;
    Vector<T>& operator=(const Vector<T>& rhs);
    Vector<T>& operator=(Vector<T>&& rhs) noexcept;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const;
    T& at(size_t id);
    const T& at(size_t id) const;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    void pushBack(const T&);
    void popBack();
    void pushFront(const T& v);
    void swap(Vector<T>& rhs) noexcept;

    void insert(size_t id, const T& t);
    void insert(size_t id, const Vector<T>& rhs, size_t beg, size_t end);
    void erase(size_t id);
    void erase(size_t beg, size_t end);

    Iterator begin();
    Iterator end();
    CIterator begin() const;
    CIterator end() const;
    CIterator cbegin() const;
    CIterator cend() const;

    Iterator insert(Iterator pos, const T& value);
    Iterator erase(Iterator pos);

    void clear();

   private:
    T* data_;
    size_t size_, capacity_;
  };
}

template<class T>
nepochatova::Vector<T>::Vector() :
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template<class T>
nepochatova::Vector<T>::~Vector()
{
  delete[] data_;
}

template<class T>
nepochatova::Vector<T>::Vector(size_t size) :
  data_(size ? new T[size] : nullptr),
  size_(size),
  capacity_(size)
{}

template<class T>
nepochatova::Vector<T>::Vector(size_t size, const T &value) : Vector(size)
{
  for (size_t i = 0; i < size; ++i) {
    data_[i] = value;
  }
}

template<class T>
nepochatova::Vector<T>::Vector(const Vector<T> &rhs) : Vector(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); ++i) {
    data_[i] = rhs.data_[i];
  }
}

template<class T>
nepochatova::Vector<T>::Vector(Vector<T> &&rhs) noexcept : Vector()
{
  swap(rhs);
}

template<class T>
nepochatova::Vector<T> &nepochatova::Vector<T>::operator=(const Vector<T> &rhs)
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector<T> cpy(rhs);
  swap(cpy);
  return *this;
}

template<class T>
nepochatova::Vector<T> &nepochatova::Vector<T>::operator=(Vector<T> &&rhs) noexcept
{
  if (this != &rhs) {
    delete[] data_;

    data_ = rhs.data_;
    size_ = rhs.size_;
    capacity_ = rhs.capacity_;

    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
  }
  return *this;
}

template<class T>
T &nepochatova::Vector<T>::operator[](size_t id) noexcept
{
  return data_[id];
}

template<class T>
const T &nepochatova::Vector<T>::operator[](size_t id) const
{
  return data_[id];
}

template<class T>
T &nepochatova::Vector<T>::at(size_t id)
{
  if (id >= getSize()) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[id];
}

template<class T>
const T &nepochatova::Vector<T>::at(size_t id) const
{
  if (id >= getSize()) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[id];
}


template<class T>
bool nepochatova::Vector<T>::isEmpty() const noexcept
{
  return !size_;
}

template<class T>
size_t nepochatova::Vector<T>::getSize() const noexcept
{
  return size_;
}

template<class T>
size_t nepochatova::Vector<T>::getCapacity() const noexcept
{
  return capacity_;
}


template<class T>
void nepochatova::Vector<T>::pushBack(const T &rhs)
{
  if (size_ >= capacity_) {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    T *newData = new T[newCapacity];
    try {
      for (size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
      }
    } catch (...) {
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
  data_[size_++] = rhs;
}

template<class T>
void nepochatova::Vector<T>::popBack()
{
  if (size_) {
    --size_;
  }
}

template<class T>
void nepochatova::Vector<T>::pushFront(const T &t)
{
  if (size_ >= capacity_) {
    size_t newCap = capacity_ ? capacity_ * 2 : 1;
    T* newData = new T[newCap];

    for (size_t i = 0; i < size_; ++i) {
      newData[i + 1] = data_[i];
    }

    newData[0] = t;

    delete[] data_;
    data_ = newData;
    capacity_ = newCap;
    ++size_;
  }
  else {
    for (size_t i = size_; i > 0; --i) {
      data_[i] = data_[i - 1];
    }
    data_[0] = t;
    ++size_;
  }
}

template<class T>
void nepochatova::Vector<T>::swap(Vector<T> &rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}


template<class T>
void nepochatova::Vector<T>::insert(size_t id, const T &t)
{
  if (id > size_) {
    throw std::out_of_range("Index out of range");
  }
  Vector<T> temp;
  for (size_t i = 0; i < id; ++i) {
    temp.pushBack(data_[i]);
  }
  temp.pushBack(t);
  for (size_t i = id; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template<class T>
void nepochatova::Vector<T>::insert(size_t id, const Vector<T> &rhs, size_t beg, size_t end)
{
  if (id > size_ || beg > end || end > rhs.getSize()) {
    throw std::out_of_range("Invalid range");
  }
  Vector<T> temp;
  for (size_t i = 0; i < id; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = beg; i < end; ++i) {
    temp.pushBack(rhs[i]);
  }
  for (size_t i = id; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template<class T>
void nepochatova::Vector<T>::erase(size_t id)
{
  if (id >= size_) {
    throw std::out_of_range("Index out of range");
  }
  Vector<T> temp;
  for (size_t i = 0; i < id; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = id + 1; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template<class T>
void nepochatova::Vector<T>::erase(size_t beg, size_t end)
{
  if (beg > end || end > size_) {
    throw std::out_of_range("Invalid range");
  }
  Vector<T> temp;
  for (size_t i = 0; i < beg; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = end; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
}

template<class T>
typename nepochatova::Vector<T>::Iterator
nepochatova::Vector<T>::begin()
{
  return Iterator(*this, 0);
}

template<class T>
typename nepochatova::Vector<T>::Iterator
nepochatova::Vector<T>::end()
{
  return Iterator(*this, size_);
}

template<class T>
typename nepochatova::Vector<T>::CIterator
nepochatova::Vector<T>::begin() const
{
  return CIterator(*this, 0);
}

template<class T>
typename nepochatova::Vector<T>::CIterator
nepochatova::Vector<T>::end() const
{
  return CIterator(*this, size_);
}

template<class T>
typename nepochatova::Vector<T>::CIterator
nepochatova::Vector<T>::cbegin() const
{
  return CIterator(*this, 0);
}

template<class T>
typename nepochatova::Vector<T>::CIterator
nepochatova::Vector<T>::cend() const
{
  return CIterator(*this, size_);
}

template<class T>
typename nepochatova::Vector<T>::Iterator
nepochatova::Vector<T>::insert(Iterator pos, const T &value)
{
  size_t index = pos.id;
  insert(index, value);
  return Iterator(*this, index);
}

template<class T>
typename nepochatova::Vector<T>::Iterator
nepochatova::Vector<T>::erase(Iterator pos)
{
  size_t index = pos.id;
  erase(index);
  return Iterator(*this, index);
}

template<class T>
void nepochatova::Vector<T>::clear()
{
  size_ = 0;
}

#endif
