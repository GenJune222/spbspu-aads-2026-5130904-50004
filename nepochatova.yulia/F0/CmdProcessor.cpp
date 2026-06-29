#include "CmdProcessor.h"
#include "XMLParser.h"

namespace nepochatova {
  void CommandProcessor::execute(const std::string &commandLine)
  {
    Vector< std::string > args = split(commandLine);

    if (args.isEmpty()) {
      return;
    }

    try {
      Command cmd = commands_.find(args[0]);
      (this->*cmd)(args);
    } catch (const std::exception &e) {
      std::cout << "<ERROR> "
          << e.what()
          << '\n';
    }
  }


  void CommandProcessor::createCmd(const Vector< std::string > &args)
  {
    if (args.getSize() != 2) {
      throw std::invalid_argument("Invalid arguments");
    }

    manager_.createTree(args[1], "xml");

    std::cout << "<OK> Tree created\n";
  }


  void CommandProcessor::dropCmd(const Vector< std::string > &args)
  {
    if (args.getSize() != 2) {
      throw std::invalid_argument("Invalid arguments");
    }

    manager_.deleteTree(args[1]);

    std::cout << "<OK> Tree removed\n";
  }


  void CommandProcessor::listCmd(const Vector< std::string > &args)
  {
    Vector< std::string > names = manager_.getTreeNames();

    for (auto name: names) {
      std::cout << name << '\n';
    }
  }


  void CommandProcessor::printTreeCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 2) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    tree->printTree();
  }


  void CommandProcessor::printSubtreeCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 3) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    tree->printSubtree(args[2]);
  }


  void CommandProcessor::findCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 4) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > result;

    tree->find(args[2], args[3], result);

    std::cout
        << args[3]
        << " ("
        << result.getSize()
        << " matches)"
        << '\n';

    for (auto node: result) {
      std::cout
          << "- "
          << node->getPath()
          << '\n';
    }
  }


  void CommandProcessor::addCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 4) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > result;

    tree->getRoot()->findByTag(args[2], result);

    if (result.isEmpty()) {
      throw std::runtime_error("No node");
    }

    tree->addNode(result[0], args[3]);

    std::cout << "<OK> Node added\n";
  }


  void CommandProcessor::deleteCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 3) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > result;

    tree->getRoot()->findById(args[2], result);

    if (result.isEmpty()) {
      throw std::runtime_error("No node");
    }

    tree->deleteNode(result[0]);

    std::cout << "<OK> Node deleted\n";
  }

  void CommandProcessor::moveCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 4) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > nodes;
    Vector< Node * > parents;

    tree->find("id", args[2], nodes);
    tree->find("id", args[3], parents);

    if (nodes.isEmpty()) {
      throw std::runtime_error("Node not found");
    }

    if (parents.isEmpty()) {
      throw std::runtime_error("Parent not found");
    }

    tree->moveNode(nodes[0], parents[0]);

    std::cout << "<OK> Node moved\n";
  }


  void CommandProcessor::renameCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 4) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > result;

    tree->find("id", args[2], result);

    if (result.isEmpty()) {
      throw std::runtime_error("Node not found");
    }

    result[0]->setTag(args[3]);

    std::cout << "<OK> Renamed\n";
  }


  void CommandProcessor::setAttributeCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 5) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > result;

    tree->find("id", args[2], result);

    if (result.isEmpty()) {
      throw std::runtime_error("Node not found");
    }

    result[0]->setAttribute(args[3], args[4]);

    std::cout << "<OK> Attribute updated\n";
  }


  void CommandProcessor::removeAttributeCmd(const Vector< std::string > &args)
  {
    if (args.getSize() < 4) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > result;

    tree->find("id", args[2], result);

    if (result.isEmpty()) {
      throw std::runtime_error("Node not found");
    }

    result[0]->removeAttribute(args[3]);

    std::cout << "<OK> Attribute removed\n";
  }


  void CommandProcessor::statsCmd(const Vector< std::string > &args)
  {
    if (args.getSize() != 3) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }


    if (args[2] == "depth") {
      std::cout
          << "MAX DEPTH: "
          << tree->getRoot()->depth()
          << '\n';
    } else if (args[2] == "size") {
      std::cout
          << "SIZE: "
          << tree->getRoot()->size()
          << '\n';
    } else {
      throw std::invalid_argument("Unknown stats type");
    }
  }


  void CommandProcessor::loadCmd(const Vector< std::string > &args)
  {
    if (args.getSize() != 3) {
      throw std::invalid_argument("Invalid arguments");
    }

    std::ifstream file(args[2]);

    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file");
    }

    DocumentTree *tree = XMLParser::load(file);

    manager_.addTree(args[1], tree);

    std::cout << "<OK> Loaded\n";
  }


  void CommandProcessor::saveCmd(const Vector< std::string > &args)
  {
    if (args.getSize() != 3) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    std::ofstream file(args[2]);

    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file");
    }

    XMLParser::save(*tree, file);

    std::cout << "<OK> Saved\n";
  }


  void CommandProcessor::insertCmd(const Vector< std::string > &args)
  {
    if (args.getSize() != 5) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *sourceTree = manager_.getTree(args[1]);
    DocumentTree *targetTree = manager_.getTree(args[3]);

    if (!sourceTree || !targetTree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > sourceNodes;

    sourceTree->find("id", args[2], sourceNodes);

    if (sourceNodes.isEmpty()) {
      throw std::runtime_error("Source node not found");
    }

    Vector< Node * > targetNodes;

    targetTree->find("id", args[4], targetNodes);

    if (targetNodes.isEmpty()) {
      throw std::runtime_error("Target node not found");
    }

    Node *copy = sourceNodes[0]->clone();
    targetNodes[0]->attachChild(copy);

    std::cout << "<OK> Subtree inserted\n";
  }


  void CommandProcessor::mergeCmd(
    const Vector<std::string> &args) {
    if (args.getSize() != 3) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *source = manager_.getTree(args[1]);
    DocumentTree *target = manager_.getTree(args[2]);

    if (!source || !target) {
      throw std::runtime_error("No tree");
    }

    Node *copy = source->getRoot()->clone();

    target->getRoot()->attachChild(copy);

    std::cout << "<OK> Documents merged\n";
  }

  void CommandProcessor::splitCmd(const Vector< std::string > &args)
  {
    if (args.getSize() != 4) {
      throw std::invalid_argument("Invalid arguments");
    }

    DocumentTree *tree = manager_.getTree(args[1]);

    if (!tree) {
      throw std::runtime_error("No tree");
    }

    Vector< Node * > nodes;

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

    DocumentTree newTree(node);

    manager_.addTree(args[3], &newTree);

    std::cout << "<OK> Document created\n";
  }
}
