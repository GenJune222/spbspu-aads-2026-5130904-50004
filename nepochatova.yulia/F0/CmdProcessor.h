#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <string>
#include "Vector.h"
#include "DocumentManager.h"

namespace nepochatova {
  class CommandProcessor
  {
  public:
    explicit CommandProcessor(DocumentManager& manager);

    void execute(const std::string &commandLine);

  private:

    using Command = void (CommandProcessor::*)(const Vector<std::string>&);

    DocumentManager &manager_;
    HashTable<std::string, Command> commands_;

    Vector<std::string> split(const std::string &line);

    void createCmd(const Vector<std::string> &args);
    void loadCmd(const Vector<std::string> &args);
    void saveCmd(const Vector<std::string> &args);
    //load и safe пока не пишем
    void dropCmd(const Vector<std::string> &args);

    void listCmd();

    void printTreeCmd(const Vector<std::string> &args);
    void printSubtreeCmd(const Vector<std::string> &args);

    void findCmd(const Vector<std::string> &args);
    void addCmd(const Vector<std::string> &args);
    void deleteCmd(const Vector<std::string> &args);
    void moveCmd(const Vector<std::string> &args);
    void renameCmd(const Vector<std::string> &args);

    void setAttributeCmd(const Vector<std::string> &args);
    void removeAttributeCmd(const Vector<std::string> &args);

    void undoCmd(const Vector<std::string> &args);
    void redoCmd(const Vector<std::string> &args);
    // для этих пока тоже

    void statsCmd(const Vector<std::string> &args);
    void analyzeCmd(const Vector<std::string> &args);
    //нужен ли вообще анализ? Крч если успею(

    void insertCmd(const Vector<std::string> &args);
    void mergeCmd(const Vector<std::string> &args);
    void splitCmd(const Vector<std::string> &args);

  };
}

#endif
