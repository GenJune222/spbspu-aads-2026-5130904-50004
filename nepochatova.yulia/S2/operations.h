#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "stack.h"
#include "queue.h"

namespace nepochatova {

  long long sum(long long a, long long b);
  long long subtract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long mod(long long a, long long b);
  long long bitwise_not(long long a);

  bool isOperator(const std::string& token);
  int getPriority(const std::string& op);

  long long applyOperation(const std::string& op, long long left, long long right);
  long long evaluateExpression(const std::string& line);
}

#endif
