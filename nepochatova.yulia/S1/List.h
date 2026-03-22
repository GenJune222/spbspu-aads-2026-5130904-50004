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
  private:
    using Node = typename List<T>::Node;
    Node* current;
    Node* head;

    explicit LIter(Node* node, Node* listHead) noexcept
      : current(node), head(listHead) {}

  public:
    LIter() noexcept: current(nullptr), head(nullptr) {}

    LIter(const LIter&) noexcept = default;
    LIter(LIter&&) noexcept = default;
    LIter& operator=(const LIter&) noexcept = default;
    LIter& operator=(LIter&&) noexcept = default;
    ~LIter() = default;
  };

  template<class T>
  class LCIter {
    friend class List<T>;
  private:
    using Node = typename List<T>::Node;
    const Node* current;
    const Node* head;

    explicit LCIter(const Node* node, const Node* listHead) noexcept
      : current(node), head(listHead) {}

  public:
    LCIter() noexcept: current(nullptr), head(nullptr) {}

    LCIter(const LCIter&) noexcept = default;
    LCIter(LCIter&&) noexcept = default;
    LCIter& operator=(const LCIter&) noexcept = default;
    LCIter& operator=(LCIter&&) noexcept = default;
    ~LCIter() = default;

    LCIter(const LIter<T>& other) noexcept: current(other.current), head(other.head) {}
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

      explicit Node(const T& val, Node* p = nullptr, Node* n = nullptr):
      data(val), prev(p), next(n) {}
      explicit Node(T&& val, Node* p = nullptr, Node* n = nullptr):
      data(std::move(val)), prev(p), next(n) {}
    };

    Node* head;
    size_t list_size;

    void clear() noexcept {
      if (head == nullptr) {
        return;
      }
      Node* start = head;
      Node* current = head->next;

      while (current != start) {
        Node* next = current->next;
        delete current;
        current = next;
      }
      delete start;
      head = nullptr;
      list_size = 0;
    }

    void copyFrom(const List& other) {
      if (other.empty()) return;

      head = new Node(other.head->data);
      head->prev = head;
      head->next = head;
      list_size = 1;

      Node* lastCreated = head;
      Node* otherCurrent = other.head->next;

      try {
        while (otherCurrent != other.head) {
          Node* newNode = new Node(otherCurrent->data);
          newNode->prev = lastCreated;
          newNode->next = head;
          lastCreated->next = newNode;
          head->prev = newNode;

          lastCreated = newNode;
          otherCurrent = otherCurrent->next;
          ++list_size;
        }
      } catch (...) {
        clear();
        throw;
      }
    }
    void swap(List& other) noexcept {
      std::swap(head, other.head);
      std::swap(list_size, other.list_size);
    }

  public:
    List():head(nullptr), list_size(0) {};

    ~List() {
      clear();
    };

    List(const List& other) : head(nullptr), list_size(0) {
      copyFrom(other);
    }

    List(List&& other) noexcept : head(other.head), list_size(other.list_size) {
      other.head = nullptr;
      other.list_size = 0;
    }

    List& operator=(const List& other) {
      List temp(other);
      this->swap(temp);
      return *this;
    }
    List& operator=(List&& other) noexcept {
      if (this != &other) {
        clear();
        head = other.head;
        list_size = other.list_size;
        other.head = nullptr;
        other.list_size = 0;
      }
      return *this;
    }

    bool empty() const noexcept {
      return list_size == 0;
    }

    size_t size() const noexcept {
      return list_size;
    }

    T& front() {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->data;
    }

    const T& front() const {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->data;
    }

    T& back() {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->prev->data;
    }

    const T& back() const {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->prev->data;
    }
  };
};
#endif
