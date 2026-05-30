#include "Graph.h"
#include "Hash_Iters.h"
#include <stdexcept>

namespace nepochatova {

  void Graph::addVertex(const std::string &name) {
    if (hasVertex(name)) return;
    vertices_.pushBack(name);
  }

  bool Graph::hasVertex(const std::string &name) const {
    for (size_t i = 0; i < vertices_.getSize(); ++i) {
      if (vertices_[i] == name) return true;
    }
    return false;
  }

  Vector<std::string> Graph::getVertices() const {
    Vector<std::string> sorted = vertices_;
    sorted.sort();
    return sorted;
  }

  void Graph::addEdge(const std::string &from, const std::string &to, unsigned long long weight) {
    addVertex(from);
    addVertex(to);

    EdgeKey key{from, to};
    if (edges_.has(key)) {
      edges_.get(key).pushBack(weight);
    } else {
      Vector<unsigned long long> weights;
      weights.pushBack(weight);
      edges_.add(key, weights);
    }
  }

  bool Graph::removeEdge(const std::string &from, const std::string &to, unsigned long long weight) {
    EdgeKey key{from, to};
    if (!edges_.has(key)) return false;

    Vector<unsigned long long> &weights = edges_.get(key);
    for (size_t i = 0; i < weights.getSize(); ++i) {
      if (weights[i] == weight) {
        weights.erase(i);
        if (weights.isEmpty()) {
          edges_.drop(key);
        }
        return true;
      }
    }
    return false;
  }

  static void sortPairsByFirst(Vector<std::pair<std::string, Vector<unsigned long long> > > &vec) {
    for (size_t i = 0; i < vec.getSize(); ++i) {
      for (size_t j = i + 1; j < vec.getSize(); ++j) {
        if (vec[j].first < vec[i].first) {
          auto tmp = vec[i];
          vec[i] = vec[j];
          vec[j] = tmp;
        }
      }
    }
  }

  Vector<std::pair<std::string, Vector<unsigned long long> > >
  Graph::getOutbound(const std::string &vertex) const {
    Vector<std::pair<std::string, Vector<unsigned long long> > > result;

    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      const EdgeKey &key = it->first;
      if (key.from == vertex) {
        result.pushBack({key.to, it->second});
      }
    }
    for (size_t i = 0; i < result.getSize(); ++i) {
      result[i].second.sort();
    }
    sortPairsByFirst(result);
    return result;
  }

  Vector<std::pair<std::string, Vector<unsigned long long> > >
  Graph::getInbound(const std::string &vertex) const {
    Vector<std::pair<std::string, Vector<unsigned long long> > > result;

    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      const EdgeKey &key = it->first;
      if (key.to == vertex) {
        result.pushBack({key.from, it->second});
      }
    }

    for (size_t i = 0; i < result.getSize(); ++i) {
      result[i].second.sort();
    }
    sortPairsByFirst(result);
    return result;
  }

  Graph Graph::merge(const Graph &other) const {
    Graph result;

    auto copyEdges = [&](const auto &table, Graph &target) {
      for (auto it = table.begin(); it != table.end(); ++it) {
        const EdgeKey &key = (*it).first;
        const Vector<unsigned long long> &weights = (*it).second;
        for (size_t w = 0; w < weights.getSize(); ++w) {
          target.addEdge(key.from, key.to, weights[w]);
        }
      }
    };

    for (size_t i = 0; i < vertices_.getSize(); ++i) result.addVertex(vertices_[i]);
    for (size_t i = 0; i < other.vertices_.getSize(); ++i) result.addVertex(other.vertices_[i]);

    copyEdges(edges_, result);
    copyEdges(other.edges_, result);
    return result;
  }

  Graph Graph::extract(const Vector<std::string> &vertices) const {
    Graph result;

    for (size_t i = 0; i < vertices.getSize(); ++i) {
      if (!hasVertex(vertices[i])) {
        throw std::out_of_range("Vertex not found in extract");
      }
      result.addVertex(vertices[i]);
    }

    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      const EdgeKey &key = it->first;
      bool fromOk = false, toOk = false;

      for (size_t i = 0; i < vertices.getSize(); ++i) {
        if (key.from == vertices[i]) fromOk = true;
        if (key.to == vertices[i]) toOk = true;
      }

      if (fromOk && toOk) {
        const Vector<unsigned long long> &weights = it->second;
        for (size_t w = 0; w < weights.getSize(); ++w) {
          result.addEdge(key.from, key.to, weights[w]);
        }
      }
    }
    return result;
  }
}
