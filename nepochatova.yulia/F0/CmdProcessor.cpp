#include "CmdProcessor.h"
#include "XMLParser.h"

namespace nepochatova {

  void CommandProcessor::createCmd(const Vector<std::string>& args)
  {
    if (args.getSize() != 2) {
      throw std::invalid_argument("Invalid create command");
    }

    manager_.createTree(args[1],"xml");

    std::cout << "<OK> Tree created\n";
  }

  void CommandProcessor::dropCmd(const Vector<std::string>& args)
  {
    if (args.getSize() != 2) {
      throw std::invalid_argument("Invalid drop command");
    }

    manager_.deleteTree(args[1]);

    std::cout << "<OK> Tree removed\n";
  }

  void CommandProcessor::listCmd(const Vector<std::string>& args)
  {
    Vector<std::string> names = manager_.getTreeNames();

    for (auto name: names){
      std::cout << name << '\n';
    }
  }

  void CommandProcessor::printTreeCmd(const Vector<std::string> &args)
  {
    if (args.getSize() < 2) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    tree->printTree();
  }


  void CommandProcessor::printSubtreeCmd(const Vector<std::string> &args)
  {
    if (args.getSize() < 3) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree* tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }
    try {
      tree->printSubtree(args[2]);
    } catch (const std::exception& e) {
      std::cout << "<NO NODE>\n";
    }
  }

  void CommandProcessor::findCmd(const Vector<std::string> &args) {
    if (args.getSize() < 4) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    Vector<Node*> result;

    tree->find(args[2], args[3], result);

    std::cout
      << "Found: "
      << result.getSize()
      << " nodes\n";

    for (auto node : result)
    {
      std::cout
          << "- "
          << node->getPath()
          << '\n';
    }
  }

  void CommandProcessor::addCmd(const Vector<std::string> &args)
  {
    if (args.getSize() < 4) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree* tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    try {
      Node* parent = tree->getRoot();

      Vector<Node*> result;
      parent->findByTag(args[2], result);

      if (result.isEmpty()) {
        std::cout << "<NO NODE>\n";
        return;
      }

      tree->addNode(result[0], args[3]);

      std::cout << "<OK> Node added\n";
    } catch (const std::exception &) {
      std::cout << "<ERROR>\n";
    }
  }

  void CommandProcessor::deleteCmd(const Vector<std::string> &args)
  {
    if (args.getSize() < 3) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    try {
      Node *root = tree->getRoot();

      Vector<Node*> result;
      root->findById(args[2], result);

      if (result.isEmpty()) {
        std::cout << "<NO NODE>\n";
        return;
      }

      tree->deleteNode(result[0]);

      std::cout << "<OK> Node deleted\n";
    } catch (const std::exception &) {
      std::cout << "<ERROR>\n";
    }
  }

  void CommandProcessor::moveCmd(const Vector<std::string> &args)
  {
    if (args.getSize() < 4) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    try {
      Node *root = tree->getRoot();

      Vector<Node*> nodes;
      root->findById(args[2], nodes);

      if (nodes.isEmpty()) {
        std::cout << "<NO NODE>\n";
        return;
      }

      Vector<Node*> parents;
      root->findById(args[3], parents);

      if (parents.isEmpty()) {
        std::cout << "<NO PARENT>\n";
        return;
      }

      tree->moveNode(nodes[0], parents[0]);

      std::cout << "<OK> Node moved\n";
    } catch (const std::exception &) {
      std::cout << "<ERROR>\n";
    }
  }

  void CommandProcessor::renameCmd(const Vector<std::string> &args)
  {
    if (args.getSize() < 4) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    try {
      Node *root = tree->getRoot();

      Vector<Node*> result;
      root->findById(args[2], result);

      if (result.isEmpty()) {
        std::cout << "<NO NODE>\n";
        return;
      }

      result[0]->setTag(args[3]);

      std::cout << "<OK> Renamed\n";
    } catch (const std::exception &) {
      std::cout << "<ERROR>\n";
    }
  }

  void CommandProcessor::setAttributeCmd(const Vector<std::string>& args)
  {
    if (args.getSize() < 5) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree* tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    Vector<Node*> result;

    tree->find("id", args[2], result);

    if (result.isEmpty()) {
      std::cout << "<NO NODE>\n";
      return;
    }

    result[0]->setAttribute(args[3], args[4]);

    std::cout << "<OK>\n";
  }

  void CommandProcessor::removeAttributeCmd(const Vector<std::string>& args)
  {
    if (args.getSize() < 4) {
      std::cout << "<INVALID ARGUMENTS>\n";
      return;
    }

    DocumentTree* tree = manager_.getTree(args[1]);

    if (!tree) {
      std::cout << "<NO TREE>\n";
      return;
    }

    Vector<Node*> result;

    tree->find("id", args[2], result);

    if (result.isEmpty()) {
      std::cout << "<NO NODE>\n";
      return;
    }

    result[0]->removeAttribute(args[3]);

    std::cout << "<OK>\n";
  }

  void CommandProcessor::statsCmd(const Vector<std::string>& args)
  {
    if (args.getSize() != 3) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree* tree = manager_.getTree(args[1]);

    if (args[2] == "depth") {
      std::cout
          << "MAX DEPTH: "
          << tree->getRoot()->depth()
          << '\n';
    }
    else if (args[2] == "size") {
      std::cout
          << "SIZE: "
          << tree->getRoot()->size()
          << '\n';
    }
    else {
      throw std::invalid_argument("Unknown stats type");
    }
  }

  void CommandProcessor::loadCmd(const Vector<std::string>& args)
  {
    if (args.getSize() != 3) {
      throw std::invalid_argument("Wrong load arguments");
    }
    std::string treeName = args[1];
    std::string filename = args[2];

    DocumentTree* tree = XMLParser::load(filename);

    manager_.addTree(treeName,tree);
  }

  void CommandProcessor::saveCmd(const Vector<std::string> &args) {

    if (args.getSize() != 3) {
      throw std::invalid_argument("Wrong save arguments");
    }

    std::string treeName = args[1];
    std::string filename = args[2];

    DocumentTree *tree = manager_.getTree(treeName);

    XMLParser parser;

    parser.save(*tree,filename);
  }

  void CommandProcessor::insertCmd(const Vector<std::string> &args)
  {
    if (args.getSize() != 5) {
      throw std::invalid_argument("Usage: insert sourceTree nodeId targetTree parentId");
    }

    DocumentTree *sourceTree = manager_.getTree(args[1]);
    DocumentTree *targetTree = manager_.getTree(args[3]);

    Vector<Node *> sourceNodes;
    sourceTree->find("id", args[2], sourceNodes);

    if (sourceNodes.isEmpty()) {
      throw std::runtime_error("Source node not found");
    }

    Vector<Node *> targetNodes;
    targetTree->find("id", args[4], targetNodes);

    if (targetNodes.isEmpty()) {
      throw std::runtime_error("Target parent not found");
    }

    Node *sourceNode = sourceNodes[0];
    Node *targetParent = targetNodes[0];

    Node *copy = sourceNode->clone();

    targetParent->attachChild(copy);
  }

  void CommandProcessor::mergeCmd(const Vector<std::string> &args)
  {
    if (args.getSize() != 3) {
      throw std::invalid_argument("Usage: merge sourceTree targetTree");
    }

    DocumentTree *source = manager_.getTree(args[1]);
    DocumentTree *target = manager_.getTree(args[2]);

    Node *sourceRoot = source->getRoot();

    Node *copy = sourceRoot->clone();

    target->getRoot()->attachChild(copy);
  }

  void CommandProcessor::splitCmd(const Vector<std::string> &args)
  {
    if (args.getSize() != 4) {
      throw std::invalid_argument("Usage: split tree nodeId newTree");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    Vector<Node *> nodes;
    tree->find("id", args[2], nodes);

    if (nodes.isEmpty()) {
      throw std::runtime_error("Node not found");
    }

    Node *node = nodes[0];

    if (node == tree->getRoot()) {
      throw std::runtime_error("Cannot split root");
    }

    Node *parent = node->getParent();
    parent->detachChild(node);

    DocumentTree *newTree = new DocumentTree(node);

    try {
      manager_.addTree(args[3],newTree);
    } catch (...) {
      delete newTree;
      throw;
    }
  }
}

