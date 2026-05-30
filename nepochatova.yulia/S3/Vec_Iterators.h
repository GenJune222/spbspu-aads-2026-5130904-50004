#ifndef VEC_ITERATORS_H
#define VEC_ITERATORS_H
#include <cstddef>

namespace nepochatova {

  template <class T> class Vector;

  template <class T>
  class Iterator {
  public:
    Iterator(Vector<T>& vec, size_t idx);

    Iterator& operator+=(size_t n);
    Iterator& operator-=(size_t n);
    T& operator*() const;

    bool operator==(const Iterator<T>& other) const;
    bool operator!=(const Iterator<T>& other) const;

    Iterator operator+(size_t n) const;
    Iterator operator-(size_t n) const;

    Vector<T>& vector;
    size_t id;
  };


  template <class T>
  class CIterator {
  public:
    CIterator(const Vector<T>& vec, size_t idx);

    CIterator& operator+=(size_t n);
    CIterator& operator-=(size_t n);
    const T& operator*() const;

    bool operator==(const CIterator<T>& other) const;
    bool operator!=(const CIterator<T>& other) const;

    CIterator operator+(size_t n) const;
    CIterator operator-(size_t n) const;

    const Vector<T>& vector;
    size_t id;
  };
}


template<class T>
nepochatova::Iterator<T>::Iterator(nepochatova::Vector<T> &vec, size_t idx)
  : vector(vec), id(idx) {}

template<class T>
nepochatova::Iterator<T> &nepochatova::Iterator<T>::operator+=(size_t n) {
  id += n;
  return *this;
}

template<class T>
nepochatova::Iterator<T> &nepochatova::Iterator<T>::operator-=(size_t n) {
  id -= n;
  return *this;
}

template<class T>
T &nepochatova::Iterator<T>::operator*() const {
  return vector[id];
}

template<class T>
bool nepochatova::Iterator<T>::operator==(const Iterator<T> &other) const {
  return id == other.id;
}

template<class T>
bool nepochatova::Iterator<T>::operator!=(const Iterator<T> &other) const {
  return id != other.id;
}

template<class T>
nepochatova::Iterator<T> nepochatova::Iterator<T>::operator+(size_t n) const {
  return nepochatova::Iterator<T>(vector, id + n);
}

template<class T>
nepochatova::Iterator<T> nepochatova::Iterator<T>::operator-(size_t n) const {
  return nepochatova::Iterator<T>(vector, id - n);
}


template<class T>
nepochatova::CIterator<T>::CIterator(const nepochatova::Vector<T> &vec, size_t idx)
  : vector(vec), id(idx) {}

template<class T>
nepochatova::CIterator<T> &nepochatova::CIterator<T>::operator+=(size_t n) {
  id += n;
  return *this;
}

template<class T>
nepochatova::CIterator<T> &nepochatova::CIterator<T>::operator-=(size_t n) {
  id -= n;
  return *this;
}

template<class T>
const T &nepochatova::CIterator<T>::operator*() const {
  return vector[id];
}

template<class T>
bool nepochatova::CIterator<T>::operator==(const CIterator<T> &other) const {
  return id == other.id;
}

template<class T>
bool nepochatova::CIterator<T>::operator!=(const CIterator<T> &other) const {
  return id != other.id;
}

template<class T>
nepochatova::CIterator<T> nepochatova::CIterator<T>::operator+(size_t n) const {
  return nepochatova::CIterator<T>(vector, id + n);
}

template<class T>
nepochatova::CIterator<T> nepochatova::CIterator<T>::operator-(size_t n) const {
  return nepochatova::CIterator<T>(vector, id - n);
}

#endif