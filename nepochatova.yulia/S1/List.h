#ifndef LIST_H
#define LIST_H
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace nepochatova {
  template<class T>
  class List;

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
  };
}
#endif
