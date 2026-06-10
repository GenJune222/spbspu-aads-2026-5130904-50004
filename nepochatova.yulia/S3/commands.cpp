#include "commands.h"
#include <utility>

namespace nepochatova {
  void printInvalid(std::ostream &out)
  {
    out << "<INVALID COMMAND>\n";
  }

  void cmdGraphs(std::istream &, std::ostream &out, GraphSet &graphs)
  {
    Vector<std::string> names;
    for (auto it = graphs.begin(); it != graphs.end(); ++it)
      names.pushBack(it->first);

    for (size_t i = 0; i < names.getSize(); ++i)
      for (size_t j = i + 1; j < names.getSize(); ++j)
        if (names[j] < names[i]) std::swap(names[i], names[j]);

    for (size_t i = 0; i < names.getSize(); ++i)
      out << names[i] << '\n';
  }

  void cmdVertexes(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string name;
    if (!(in >> name) || !graphs.has(name)) {
      printInvalid(out);
      return;
    }

    Vector<std::string> verts = graphs.get(name).getVertices();
    for (size_t i = 0; i < verts.getSize(); ++i)
      out << verts[i] << '\n';
  }

  void cmdOutbound(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string gname, v;
    if (!(in >> gname >> v) || !graphs.has(gname)) {
      printInvalid(out);
      return;
    }

    const Graph &g = graphs.get(gname);
    if (!g.hasVertex(v)) {
      printInvalid(out);
      return;
    }

    auto res = g.getOutbound(v);
    for (size_t i = 0; i < res.getSize(); ++i) {
      out << res[i].first;
      for (size_t w = 0; w < res[i].second.getSize(); ++w)
        out << ' ' << res[i].second[w];
      out << '\n';
    }
  }

  void cmdInbound(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string gname, v;
    if (!(in >> gname >> v) || !graphs.has(gname)) {
      printInvalid(out);
      return;
    }

    const Graph &g = graphs.get(gname);
    if (!g.hasVertex(v)) {
      printInvalid(out);
      return;
    }

    auto res = g.getInbound(v);
    for (size_t i = 0; i < res.getSize(); ++i) {
      out << res[i].first;
      for (size_t w = 0; w < res[i].second.getSize(); ++w)
        out << ' ' << res[i].second[w];
      out << '\n';
    }
  }

  void cmdBind(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string gname, a, b;
    unsigned long long w;
    if (!(in >> gname >> a >> b >> w) || !graphs.has(gname)) {
      printInvalid(out);
      return;
    }
    graphs.get(gname).addEdge(a, b, w);
  }

  void cmdCut(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string gname, a, b;
    unsigned long long w;
    if (!(in >> gname >> a >> b >> w) || !graphs.has(gname)) {
      printInvalid(out);
      return;
    }
    if (!graphs.get(gname).removeEdge(a, b, w)) printInvalid(out);
  }

  void cmdCreate(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string name;
    if (!(in >> name) || graphs.has(name)) {
      printInvalid(out);
      return;
    }

    size_t k;
    if (!(in >> k)) {
      printInvalid(out);
      return;
    }

    Graph g;
    for (size_t i = 0; i < k; ++i) {
      std::string v;
      if (!(in >> v)) {
        printInvalid(out);
        return;
      }
      g.addVertex(v);
    }
    graphs.add(name, g);
  }

  void cmdMerge(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string newname, old1, old2;
    if (!(in >> newname >> old1 >> old2) || graphs.has(newname)) {
      printInvalid(out);
      return;
    }
    if (!graphs.has(old1) || !graphs.has(old2)) {
      printInvalid(out);
      return;
    }
    graphs.add(newname, graphs.get(old1).merge(graphs.get(old2)));
  }

  void cmdExtract(std::istream &in, std::ostream &out, GraphSet &graphs)
  {
    std::string newname, oldname;
    if (!(in >> newname >> oldname) || graphs.has(newname)) {
      printInvalid(out);
      return;
    }
    if (!graphs.has(oldname)) {
      printInvalid(out);
      return;
    }

    size_t k;
    if (!(in >> k)) {
      printInvalid(out);
      return;
    }

    Vector<std::string> verts;
    for (size_t i = 0; i < k; ++i) {
      std::string v;
      if (!(in >> v)) {
        printInvalid(out);
        return;
      }
      verts.pushBack(v);
    }

    const Graph &old = graphs.get(oldname);
    for (size_t i = 0; i < verts.getSize(); ++i)
      if (!old.hasVertex(verts[i])) {
        printInvalid(out);
        return;
      }

    graphs.add(newname, old.extract(verts));
  }

}
