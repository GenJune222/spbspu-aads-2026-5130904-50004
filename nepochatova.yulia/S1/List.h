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
  };
};
#endif
