#ifndef STACK_HPP
#define STACK_HPP

#include "../common/List.hpp"
#include <stdexcept>
#include <utility>

namespace nepochatova {
  template < class T >
  class Stack {
  private:
    List< T > data_;

  public:
    void push(const T& value);
    T drop();
    T & top();
    const T & top() const;
    bool empty() const noexcept;
    size_t size() const noexcept;
    void clear();
  };

template < class T >
 void Stack<T>::push(const T& value) {
  data_.push_back(value);
}

template < class T >
T Stack<T>::drop() {
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  T value = std::move(data_.back());
  data_.pop_back();
  return value;
}

template < class T >
T& Stack<T>::top() {
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  return data_.back();
}

template < class T >
const T& Stack<T>::top() const {
  if (empty()) {
    throw std::out_of_range("Stack is empty");
  }
  return data_.back();
}

template < class T >
bool Stack<T>::empty() const noexcept {
  return data_.empty();
}

template < class T >
size_t Stack<T>::size() const noexcept {
  return data_.size();
}

template < class T >
void Stack<T>::clear() {
  data_.clear();
}
}
#endif