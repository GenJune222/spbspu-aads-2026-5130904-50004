#ifndef STACK_HPP
#define STACK_HPP

#include "../common/BiList.hpp"

namespace nepochatova
{
  template < class T >
  class Stack
  {
  private:
    List< T > data_;

  public:
    void push(const T& value);
    T drop();
    T & top();
    const T & top() const;
    bool empty() const;
    size_t size() const;
    void clear();
  };
}

#endif