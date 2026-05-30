#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <utility>
#include "HashTable.h"


namespace nepochatova {
  struct EdgeKey {
    std::string from;
    std::string to;

    bool operator==(const EdgeKey &other) const noexcept {
      return from == other.from && to == other.to;
    }
  };

  struct EdgeKeyHash {
    size_t operator()(const EdgeKey &k) const noexcept {
      size_t h1 = 14695981039346656037ULL;
      for (char c: k.from) {
        h1 ^= static_cast<size_t>(c);
        h1 *= 1099511628211ULL;
      }
      size_t h2 = 14695981039346656037ULL;
      for (char c: k.to) {
        h2 ^= static_cast<size_t>(c);
        h2 *= 1099511628211ULL;
      }
      return h1 ^ (h2 << 1);
    }
  };

  struct EdgeKeyEqual {
    bool operator()(const EdgeKey &a, const EdgeKey &b) const noexcept {
      return a == b;
    }
  };

  class Graph {
  public:
    Graph() = default;

    explicit Graph(const std::string &name);
    void addVertex(const std::string &name);
    bool hasVertex(const std::string &name) const;
    Vector<std::string> getVertices() const;
    void addEdge(const std::string &from, const std::string &to, unsigned long long weight);
    bool removeEdge(const std::string &from, const std::string &to, unsigned long long weight);

    Vector<std::pair<std::string, Vector<unsigned long long> > >
    getOutbound(const std::string &vertex) const;

    Vector<std::pair<std::string, Vector<unsigned long long> > >
    getInbound(const std::string &vertex) const;

    Graph merge(const Graph &other) const;
    Graph extract(const Vector<std::string> &vertices) const;

    void swap(Graph &other) noexcept;
    const std::string &getName() const noexcept;

  private:
    std::string name_;
    Vector<std::string> vertices_;
    HashTable<EdgeKey, Vector<unsigned long long>, EdgeKeyHash, EdgeKeyEqual> edges_;
  };
}
#endif

