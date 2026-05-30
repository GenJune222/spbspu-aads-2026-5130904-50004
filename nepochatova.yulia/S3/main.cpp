#include "HashTable.h"
#include "Hash_Iters.h"
#include "Graph.h"
#include "commands.h"
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: cannot open file " << argv[1] << "\n";
    return 1;
  }

  nepochatova::Graph graphs;

  std::string graphName;
  size_t edgeCount;

  while (file >> graphName >> edgeCount) {
    nepochatova::Graph g;
    for (size_t i = 0; i < edgeCount; ++i) {
      std::string from, to;
      unsigned long long weight;
      if (!(file >> from >> to >> weight)) {
        std::cerr << "Error reading edges for graph " << graphName << "\n";
        return 1;
      }
      g.addEdge(from, to, weight);
    }
    try {
      graphs.add(graphName, std::move(g));
    } catch (const std::exception&) {
      std::cerr << "Error adding graph " << graphName << "\n";
      return 1;
    }
  }

  nepochatova::HashTable<std::string, void(*)(std::istream&, std::ostream&, nepochatova::GraphSet&)> commands(17);

  commands.add("graphs", nepochatova::cmdGraphs);
  commands.add("vertexes", nepochatova::cmdVertexes);
  commands.add("outbound", nepochatova::cmdOutbound);
  commands.add("inbound", nepochatova::cmdInbound);
  commands.add("bind", nepochatova::cmdBind);
  commands.add("cut", nepochatova::cmdCut);
  commands.add("create", nepochatova::cmdCreate);
  commands.add("merge", nepochatova::cmdMerge);
  commands.add("extract", nepochatova::cmdExtract);

  std::string cmd;
  while (std::cin >> cmd) {
    if (!commands.has(cmd)) {
      nepochatova::printInvalid(std::cout);
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }

    try {
      commands.get(cmd)(std::cin, std::cout, graphs);
    } catch (const std::exception&) {
      nepochatova::printInvalid(std::cout);
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}

