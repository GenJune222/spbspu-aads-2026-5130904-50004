#ifndef LIST_H
#define LIST_H
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace nepochatova {
  template<class T> class List;
  template<class T>
  class LIter {
    friend class List<T>;

  };

  template<class T>
  class LCIter {
    friend class List<T>;
  };

  template<class T>
  class List {
    friend class LIter< T >;
    friend class LCIter< T >;

  private:
    struct Node {
      T data;
      Node* next;
      Node* prev;

      Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    size_t list_size;
  };
};
#endif
