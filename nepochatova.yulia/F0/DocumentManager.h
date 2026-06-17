#ifndef DOCUMENT_MANAGER_H
#define DOCUMENT_MANAGER_H

#include <string>
#include <stdexcept>
#include "DocumentTree.h"
#include "HashTable.h"

namespace nepochatova {
  class DocumentManager
  {
  public:
    DocumentManager();
    ~DocumentManager();
    DocumentManager(const DocumentManager &) = delete;
    DocumentManager &operator=(const DocumentManager &) = delete;

    void createTree(const std::string &name, const std::string &rootTag);
    void deleteTree(const std::string &name);
    bool contains(const std::string &name) const;

    DocumentTree* getTree(const std::string &name);
    const DocumentTree* getTree(const std::string &name) const;
    Vector<std::string> getTreeNames() const;

    size_t size() const;

  private:
    HashTable<std::string, DocumentTree* > trees_;
  };
}

#endif
