#include "DocumentManager.h"

namespace nepochatova {

  DocumentManager::DocumentManager() {}

  DocumentManager::~DocumentManager()
  {
    for (auto it = trees_.begin(); it != trees_.end(); ++it) {
      delete it->second;
    }
  }

  void DocumentManager::createTree(const std::string &name, const std::string &rootTag)
  {
    if (trees_.contains(name)) {
      throw std::runtime_error("tree already exists");
    }

    DocumentTree* tree = new DocumentTree(rootTag);

    try {
      trees_.insert(name, tree);
    } catch (...) {
      delete tree;
      throw;
    }
  }

  void DocumentManager::addTree(const std::string& name, DocumentTree* tree)
  {
    if (!tree) {
      throw std::invalid_argument("null tree");
    }

    if (contains(name)) {
      throw std::invalid_argument("tree already exists");
    }

    trees_.insert(name, tree);
  }

  void DocumentManager::deleteTree(const std::string &name)
  {
    if (!trees_.contains(name)) {
      throw std::runtime_error("tree not found");
    }

    DocumentTree* tree = trees_.erase(name);

    delete tree;
  }

  bool DocumentManager::contains(const std::string &name) const noexcept
  {
    return trees_.contains(name);
  }

  DocumentTree *DocumentManager::getTree(const std::string &name)
  {
    if (!trees_.contains(name)) {
      throw std::runtime_error("tree not found");
    }
    return trees_.find(name);
  }

  const DocumentTree *DocumentManager::getTree(const std::string &name) const
  {
    if (!trees_.contains(name)) {
      throw std::runtime_error("tree not found");
    }
    return trees_.find(name);
  }

  Vector<std::string> DocumentManager::getTreeNames() const
  {
    Vector< std::string > result;

    for (auto it = trees_.begin(); it != trees_.end(); ++it)
    {
      result.pushBack(it->first);
    }

    return result;
  }

  size_t DocumentManager::size() const noexcept
  {
    return trees_.size();
  }
}
