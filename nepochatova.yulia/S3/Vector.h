#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "Vec_Iterators.h"

namespace nepochatova {

  template <class T>
  class Vector {
   public:
    Vector();
    ~Vector();
    explicit Vector(size_t size);
    Vector(size_t size, const T& value);

    Vector(const Vector<T>& rhs);
    Vector(Vector<T>&& rhs) noexcept;
    Vector<T>& operator=(const Vector<T>& rhs);
    Vector<T>& operator=(Vector<T>&& rhs) noexcept;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
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

    Iterator<T> begin();
    Iterator<T> end();
    CIterator<T> begin() const;
    CIterator<T> end() const;
    CIterator<T> cbegin() const;
    CIterator<T> cend() const;

    Iterator<T> insert(Iterator<T> pos, const T& value);
    Iterator<T> insert(Iterator<T> pos, CIterator<T> beg, CIterator<T> end);
    Iterator<T> insert(Iterator<T> pos, size_t count, const T& value);
    Iterator<T> erase(Iterator<T> pos);
    Iterator<T> erase(Iterator<T> beg, Iterator<T> end);
    Iterator<T> erase(CIterator<T> beg, CIterator<T> end);

    void sort();

   private:
    T* data_;
    size_t size_, capacity_;

    void quickSort(size_t low, size_t high);
    size_t partition(size_t low, size_t high);
  };

}


template<class T>
nepochatova::Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {
}

template<class T>
nepochatova::Vector<T>::~Vector() {
  delete[] data_;
}

template<class T>
nepochatova::Vector<T>::Vector(size_t size) : data_(size ? new T[size] : nullptr), size_(size), capacity_(size) {
}

template<class T>
nepochatova::Vector<T>::Vector(size_t size, const T &value) : Vector(size) {
  for (size_t i = 0; i < size; ++i) {
    data_[i] = value;
  }
}

template<class T>
nepochatova::Vector<T>::Vector(const Vector<T> &rhs) : Vector(rhs.getSize()) {
  for (size_t i = 0; i < rhs.getSize(); ++i) {
    data_[i] = rhs.data_[i];
  }
}

template<class T>
nepochatova::Vector<T>::Vector(Vector<T> &&rhs) noexcept : Vector() {
  swap(rhs);
}

template<class T>
nepochatova::Vector<T> &nepochatova::Vector<T>::operator=(const Vector<T> &rhs) {
  if (this == std::addressof(rhs)) {
    return *this;
  }
  Vector<T> cpy(rhs);
  swap(cpy);
  return *this;
}

template<class T>
nepochatova::Vector<T> &nepochatova::Vector<T>::operator=(Vector<T> &&rhs) noexcept {
  Vector<T> cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template<class T>
T &nepochatova::Vector<T>::operator[](size_t id) noexcept {
  return const_cast<T &>((*static_cast<const Vector<T> *>(this))[id]);
}

template<class T>
const T &nepochatova::Vector<T>::operator[](size_t id) const noexcept {
  return data_[id];
}

template<class T>
T &nepochatova::Vector<T>::at(size_t id) {
  if (id >= getSize()) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[id];
}

template<class T>
const T &nepochatova::Vector<T>::at(size_t id) const {
  if (id >= getSize()) {
    throw std::out_of_range("Index out of range");
  }
  return (*this)[id];
}


template<class T>
bool nepochatova::Vector<T>::isEmpty() const noexcept { return !size_; }

template<class T>
size_t nepochatova::Vector<T>::getSize() const noexcept { return size_; }

template<class T>
size_t nepochatova::Vector<T>::getCapacity() const noexcept { return capacity_; }


template<class T>
void nepochatova::Vector<T>::pushBack(const T &rhs) {
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
void nepochatova::Vector<T>::popBack() {
  if (size_) {
    --size_;
  }
}

template<class T>
void nepochatova::Vector<T>::pushFront(const T &t) {
  Vector<T> v(getSize() + 1);
  v[0] = t;
  for (size_t i = 1; i < v.getSize(); ++i) {
    v[i] = (*this)[i - 1];
  }
  swap(v);
}

template<class T>
void nepochatova::Vector<T>::swap(Vector<T> &rhs) noexcept {
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}


template<class T>
void nepochatova::Vector<T>::insert(size_t id, const T &t) {
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
void nepochatova::Vector<T>::insert(size_t id, const Vector<T> &rhs, size_t beg, size_t end) {
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
void nepochatova::Vector<T>::erase(size_t id) {
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
void nepochatova::Vector<T>::erase(size_t beg, size_t end) {
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
nepochatova::Iterator<T> nepochatova::Vector<T>::begin() {
  return Iterator<T>(*this, 0);
}

template<class T>
nepochatova::Iterator<T> nepochatova::Vector<T>::end() {
  return Iterator<T>(*this, size_);
}

template<class T>
nepochatova::CIterator<T> nepochatova::Vector<T>::begin() const {
  return CIterator<T>(*this, 0);
}

template<class T>
nepochatova::CIterator<T> nepochatova::Vector<T>::end() const {
  return CIterator<T>(*this, size_);
}

template<class T>
nepochatova::CIterator<T> nepochatova::Vector<T>::cbegin() const {
  return CIterator<T>(*this, 0);
}

template<class T>
nepochatova::CIterator<T> nepochatova::Vector<T>::cend() const {
  return CIterator<T>(*this, size_);
}


template<class T>
nepochatova::Iterator<T> nepochatova::Vector<T>::insert(Iterator<T> pos, const T &value) {
  size_t index = pos.id;
  insert(index, value);
  return Iterator<T>(*this, index);
}

template<class T>
nepochatova::Iterator<T> nepochatova::Vector<T>::insert(Iterator<T> pos, CIterator<T> beg, CIterator<T> end) {
  if (beg.id == end.id) {
    return pos;
  }
  size_t index = pos.id;
  Vector<T> temp;
  for (size_t i = 0; i < index; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = beg.id; i < end.id; ++i) {
    temp.pushBack(beg.vector[i]);
  }
  for (size_t i = index; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
  return Iterator<T>(*this, index);
}

template<class T>
nepochatova::Iterator<T> nepochatova::Vector<T>::insert(Iterator<T> pos, size_t count, const T &value) {
  if (count == 0) {
    return pos;
  }
  size_t index = pos.id;
  Vector<T> temp;
  for (size_t i = 0; i < index; ++i) {
    temp.pushBack(data_[i]);
  }
  for (size_t i = 0; i < count; ++i) {
    temp.pushBack(value);
  }
  for (size_t i = index; i < size_; ++i) {
    temp.pushBack(data_[i]);
  }
  swap(temp);
  return Iterator<T>(*this, index);
}

template<class T>
nepochatova::Iterator<T> nepochatova::Vector<T>::erase(Iterator<T> pos) {
  size_t index = pos.id;
  erase(index);
  return Iterator<T>(*this, index);
}

template<class T>
nepochatova::Iterator<T> nepochatova::Vector<T>::erase(Iterator<T> beg, Iterator<T> end) {
  if (beg.id == end.id) {
    return beg;
  }
  size_t b = beg.id, e = end.id;
  erase(b, e);
  return Iterator<T>(*this, b);
}

template<class T>
nepochatova::Iterator<T> nepochatova::Vector<T>::erase(CIterator<T> beg, CIterator<T> end) {
  if (beg.id == end.id) {
    return Iterator<T>(*this, beg.id);
  }
  size_t b = beg.id, e = end.id;
  erase(b, e);
  return Iterator<T>(*this, b);
}


template<class T>
void nepochatova::Vector<T>::sort() {
  if (size_ <= 1) {
    return;
  }
  quickSort(0, size_ - 1);
}

template<class T>
size_t nepochatova::Vector<T>::partition(size_t low, size_t high) {
  T pivot = data_[high];
  size_t i = low;
  for (size_t j = low; j < high; ++j) {
    if (data_[j] < pivot) {
      T tmp = data_[i];
      data_[i] = data_[j];
      data_[j] = tmp;
      ++i;
    }
  }
  T tmp = data_[i];
  data_[i] = data_[high];
  data_[high] = tmp;
  return i;
}

template<class T>
void nepochatova::Vector<T>::quickSort(size_t low, size_t high) {
  if (low < high) {
    size_t pi = partition(low, high);
    if (pi > 0) {
      quickSort(low, pi - 1);
    }
    quickSort(pi + 1, high);
  }
}

#endif
