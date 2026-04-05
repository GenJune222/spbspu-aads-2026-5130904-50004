#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/BiList.hpp"

namespace nepochatova
{
  template < class T >
  class Queue
  {
  private:
    List< T > data_;

  public:
    void push(const T& value);
    T drop();
    T & front();
    const T & front() const;
    bool empty() const;
    size_t size() const;
    void clear();
  };
}

#endif