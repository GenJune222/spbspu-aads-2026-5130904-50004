#ifndef LIST_H
#define LIST_H
#include "ListIters.h"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace nepochatova{

  template< class T >
  class List
  {
    friend class LIter<T>;
    friend class LCIter<T>;

    struct Node {
      T data;
      Node* next;
      Node* prev;

      explicit Node(const T& val, Node* n = nullptr, Node* p = nullptr):
        data(val),
        next(n),
        prev(p)
      {}
      explicit Node(T&& val, Node* n = nullptr, Node* p = nullptr):
        data(std::move(val)),
        next(n),
        prev(p)
      {}
    };

    Node* head;
    size_t list_size;

    void copyFrom(const List& other)
    {
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
    void swap(List& other) noexcept
    {
      std::swap(head, other.head);
      std::swap(list_size, other.list_size);
    }
  public:
    List(): head(nullptr), list_size(0) {};

    ~List()
    {
      clear();
    };

    List(const List& other): head(nullptr), list_size(0)
    {
      for (auto it = other.cbegin(); it != other.cend(); ++it) {
        push_back(*it);
      }
    }

    List(List&& other) noexcept: head(other.head), list_size(other.list_size)
    {
      other.head = nullptr;
      other.list_size = 0;
    }

    List& operator=(const List& other)
    {
      List temp(other);
      this->swap(temp);
      return *this;
    }
    List& operator=(List&& other) noexcept
    {
      if (this != &other) {
        clear();
        head = other.head;
        list_size = other.list_size;
        other.head = nullptr;
        other.list_size = 0;
      }
      return *this;
    }

    LIter<T> begin()
    {
      return LIter<T>(head, head);
    }

    LIter<T> end()
    {
      return LIter<T>(nullptr, head);
    }

    LCIter<T> cbegin() const
    {
      return LCIter<T>(head, head);
    }

    LCIter<T> cend() const
    {
      return LCIter<T>(nullptr, head);
    }

    bool empty() const noexcept
    {
      return list_size == 0;
    }

    size_t size() const noexcept
    {
      return list_size;
    }

    T& front()
    {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->data;
    }

    const T& front() const
    {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->data;
    }

    T& back()
    {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->prev->data;
    }

    const T& back() const
    {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      return head->prev->data;
    }

    void clear() noexcept
    {
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

    void push_front(const T& value)
    {
      Node* new_node = new Node(value);
      if (empty()) {
        head = new_node;
        head->prev = head;
        head->next = head;
      } else {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
        head = new_node;
      }
      ++list_size;
    }

    void push_front(T&& value)
    {
      Node* new_node = new Node(std::move(value));
      if (empty()) {
        head = new_node;
        head->prev = head;
        head->next = head;
      } else {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
        head = new_node;
      }
      ++list_size;
    }

    void push_back(const T& value)
    {
      Node* new_node = new Node(value);
      if (empty()) {
        head = new_node;
        head->prev = head;
        head->next = head;
      } else {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
      }
      ++list_size;
    }

    void push_back(T&& value)
    {
      Node* new_node = new Node(std::move(value));
      if (empty()) {
        head = new_node;
        head->prev = head;
        head->next = head;
      } else {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
      }
      ++list_size;
    }

    void pop_front()
    {
      if (empty()) {
        throw std::runtime_error("List is empty");
      } else if (size() == 1) {
        delete head;
        head = nullptr;
        list_size = 0;
      } else {
        Node* toDelete = head;
        head->prev->next = head->next;
        head->next->prev = head->prev;
        head = head->next;
        delete toDelete;
        --list_size;
      }
    }

    void pop_back()
    {
      if (empty()) {
        throw std::runtime_error("List is empty");
      } else if (size() == 1) {
        delete head;
        head = nullptr;
        list_size = 0;
      } else {
        Node* toDelete = head->prev;
        toDelete->prev->next = head;
        head->prev = toDelete->prev;
        delete toDelete;
        --list_size;
      }
    }

    LIter<T> insert(LIter<T> pos, const T& value)
    {
      if (empty()) {
        push_back(value);
        return begin();
      }
      Node* curr = pos.current;
      if (curr == nullptr) {
        push_back(value);
        return LIter<T>(head->prev, head);
      }
      Node* new_node = new Node(value, curr, curr->prev);
      curr->prev->next = new_node;
      curr->prev = new_node;
      ++list_size;
      return LIter<T>(new_node, head);
    }

    LIter<T> insert(LIter<T> pos, T&& value)
    {
      if (empty()) {
        push_back(std::move(value));
        return begin();
      }
      Node* curr = pos.current;
      if (curr == nullptr) {
        push_back(std::move(value));
        return LIter<T>(head->prev, head);
      }
      Node* new_node = new Node(std::move(value), curr, curr->prev);
      curr->prev->next = new_node;
      curr->prev = new_node;
      ++list_size;
      return LIter<T>(new_node, head);
    }

    LIter<T> erase(LIter<T> pos)
    {
      if (empty()) {
        throw std::runtime_error("List is empty");
      }
      if (pos.current == nullptr){
        throw std::runtime_error("Cannot erase end()");
      }
      if (size() == 1) {
        delete pos.current;
        head = nullptr;
        list_size = 0;
        return end();
      }
      Node* toDelete = pos.current;
      Node* nextNode = toDelete->next;
      if (toDelete == head){
        head = head->next;
      }
      toDelete->prev->next = toDelete->next;
      toDelete->next->prev = toDelete->prev;
      delete toDelete;
      --list_size;
      return LIter<T>(nextNode == head ? nullptr : nextNode, head);
    }
  };
};
#endif
