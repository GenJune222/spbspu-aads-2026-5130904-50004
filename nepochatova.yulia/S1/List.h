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

    T& operator*() const {
      return current->data;
    }

    T* operator->() const {
      return &(current->data);
    }

    LIter& operator++() noexcept {
      if (current) {
        current = current->next;
        if (current == head) {
          current = nullptr;
        }
      }
      return *this;
    }

    LIter operator++(int) noexcept {
      LIter temp = *this;
      ++(*this);
      return temp;
    }

    LIter& operator--() noexcept {
      if (!current) {
        if (head) {
          current = head->prev;
        }
      } else {
        current = current->prev;
      }
      return *this;
    }

    LIter operator--(int) noexcept {
      LIter temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const LIter& other) const noexcept {
      return current == other.current;
    }

    bool operator!=(const LIter& other) const noexcept {
      return current != other.current;
    }
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

    const T& operator*() const {
      return current->data;
    }

    const T* operator->() const {
      return &(current->data);
    }

    LCIter& operator++() noexcept {
      if (current) {
        current = current->next;
        if (current == head) {
          current = nullptr;
        }
      }
      return *this;
    }

    LCIter operator++(int) noexcept {
      LCIter temp = *this;
      ++(*this);
      return temp;
    }

    LCIter& operator--() noexcept {
      if (!current) {
        if (head) {
          current = head->prev;
        }
      } else {
        current = current->prev;
      }
      return *this;
    }

    LCIter operator--(int) noexcept {
      LCIter temp = *this;
      --(*this);
      return temp;
    }

    bool operator==(const LCIter& other) const noexcept {
      return current == other.current;
    }

    bool operator!=(const LCIter& other) const noexcept {
      return current != other.current;
    }
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

    LIter<T> begin() {
      return LIter<T>(head, head);
    }

    LIter<T> end() {
      return LIter<T>(nullptr, head);
    }

    LCIter<T> cbegin() const {
      return LCIter<T>(head, head);
    }

    LCIter<T> cend() const {
      return LCIter<T>(nullptr, head);
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

    void push_front(const T& value)
    {
      Node* new_node = new Node(value);
      if (empty())
      {
        head = new_node;
        head->prev = head;
        head->next = head;
      }
      else
      {
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
      if (empty())
      {
        head = new_node;
        head->prev = head;
        head->next = head;
      }
      else
      {
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
      if (empty())
      {
        head = new_node;
        head->prev = head;
        head->next = head;
      }
      else
      {
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
      if (empty())
      {
        head = new_node;
        head->prev = head;
        head->next = head;
      }
      else
      {
        new_node->prev = head->prev;
        new_node->next = head;
        head->prev->next = new_node;
        head->prev = new_node;
      }
      ++list_size;
    }

    void pop_front()
    {
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      else if (size() == 1)
      {
        delete head;
        head = nullptr;
        list_size = 0;
      }
      else
      {
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
      if (empty())
      {
        throw std::runtime_error("List is empty");
      }
      else if (size() == 1)
      {
        delete head;
        head = nullptr;
        list_size = 0;
      }
      else
      {
        Node* toDelete = head->prev;
        toDelete->prev->next = head;
        head->prev = toDelete->prev;
        delete toDelete;
        --list_size;
      }
    }
  };
};
#endif
