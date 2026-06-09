#ifndef LISTITERS_H
#define LISTITERS_H

#include <iterator>

namespace nepochatova {
  template< class T >
  class List;

  template< class T >
  class LIter : public std::iterator<std::bidirectional_iterator_tag, T>
  {
    friend class List<T>;

    using Node = typename List<T>::Node;
    Node* current;
    Node* head;

    explicit LIter(Node* node, Node* listHead) noexcept:
      current(node),
      head(listHead)
    {}

  public:
    LIter() noexcept: current(nullptr), head(nullptr) {}

    LIter(const LIter&) noexcept = default;
    LIter(LIter&&) noexcept = default;
    LIter& operator=(const LIter&) noexcept = default;
    LIter& operator=(LIter&&) noexcept = default;
    ~LIter() = default;

    T& operator*() const
    {
      return current->data;
    }

    T* operator->() const
    {
      return &(current->data);
    }

    LIter& operator++() noexcept
    {
      if (current) {
        current = current->next;
        if (current == head) {
          current = nullptr;
        }
      }
      return *this;
    }

    LIter operator++(int) noexcept
    {
      LIter temp = *this;
      ++(*this);
      return temp;
    }

    LIter& operator--() noexcept
    {
      if (!current) {
        if (head) {
          current = head->prev;
        }
      } else {
        current = current->prev;
      }
      return *this;
    }

    LIter operator--(int) noexcept
    {
      LIter temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const LIter& other) const noexcept
    {
      return current == other.current;
    }

    bool operator!=(const LIter& other) const noexcept
    {
      return current != other.current;
    }
  };

  template< class T >
  class LCIter : public std::iterator<std::bidirectional_iterator_tag, const T>
  {
    friend class List<T>;

    using Node = typename List<T>::Node;
    const Node* current;
    const Node* head;

    explicit LCIter(const Node* node, const Node* listHead) noexcept:
      current(node),
      head(listHead)
    {}

  public:
    LCIter() noexcept: current(nullptr), head(nullptr) {}

    LCIter(const LCIter&) noexcept = default;
    LCIter(LCIter&&) noexcept = default;
    LCIter& operator=(const LCIter&) noexcept = default;
    LCIter& operator=(LCIter&&) noexcept = default;
    ~LCIter() = default;

    LCIter(const LIter<T>& other) noexcept:
      current(other.current),
      head(other.head)
    {}

    const T& operator*() const
    {
      return current->data;
    }

    const T* operator->() const
    {
      return &(current->data);
    }

    LCIter& operator++() noexcept
    {
      if (current) {
        current = current->next;
        if (current == head) {
          current = nullptr;
        }
      }
      return *this;
    }

    LCIter operator++(int) noexcept
    {
      LCIter temp = *this;
      ++(*this);
      return temp;
    }

    LCIter& operator--() noexcept
    {
      if (!current) {
        if (head) {
          current = head->prev;
        }
      } else {
        current = current->prev;
      }
      return *this;
    }

    LCIter operator--(int) noexcept
    {
      LCIter temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return current == other.current;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return current != other.current;
    }
  };
}
#endif
