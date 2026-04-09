#include "operations.h"
#include "stack.h"
#include "queue.h"
#include <string>
#include <stdexcept>

namespace nepochatova {

long long sum(long long a, long long b) {
  return a + b;
}

long long subtract(long long a, long long b) {
  return a - b;
}

long long multiply(long long a, long long b) {
  return a * b;
}

long long divide(long long a, long long b) {
  if (b == 0) {
    throw std::runtime_error("Division by zero");
  }
  return a / b;
}

long long mod(long long a, long long b) {
  if (b == 0) {
    throw std::runtime_error("Module by zero");
  }
  return a % b;
}

long long bitwise_not(long long a) {
  return !a;
}

bool isOperator(const std::string& token) {
  return token == "+" || token == "-" || token == "*" ||
         token == "/" || token == "%" || token == "!";
}

int getPriority(const std::string& op) {
  if (op == "+" || op == "-") {
    return 1;
  }
  if (op == "*" || op == "/" || op == "%") {
    return 2;
  }
  if (op == "!") {
    return 3;
  }
  return 0;
}

void applyOperation(const std::string& op, Stack<long long>& values) {
  if (op == "!") {
    if (values.empty()) {
      throw std::runtime_error("Invalid expression: missing operand for !");
    }
    long long a = values.drop();
    values.push(bitwise_not(a));
    return;
  }

  if (values.size() < 2) {
    throw std::runtime_error("Invalid expression: missing operands");
  }

  long long b = values.drop();
  long long a = values.drop();
  long long res = 0;

  if (op == "+") {
    res = sum(a, b);
  } else if (op == "-") {
    res = subtract(a, b);
  } else if (op == "*") {
    res = multiply(a, b);
  } else if (op == "/") {
    res = divide(a, b);
  } else if (op == "%") {
    res = mod(a, b);
  } else {
    throw std::runtime_error("Unknown operator: " + op);
  }

  values.push(res);
}

  Queue<std::string> infixToPostfix(const std::string& line) {
  Queue<std::string> output;
  Stack<std::string> ops;
  std::string token;
  size_t start = 0;
  size_t end = 0;

  while (end != std::string::npos) {
    end = line.find(' ', start);
    token = line.substr(start, end - start);
    start = (end == std::string::npos) ? std::string::npos : end + 1;
    if (token.empty()) continue;

    if (token == "(") {
      ops.push(token);
    } else if (token == ")") {
      while (!ops.empty() && ops.top() != "(") {
        output.push(ops.drop());
      }
      if (ops.empty()) throw std::runtime_error("Mismatched parentheses");
      ops.drop();
    } else if (isOperator(token)) {
      while (!ops.empty() && ops.top() != "(" &&
             getPriority(ops.top()) >= getPriority(token)) {
        output.push(ops.drop());
             }
      ops.push(token);
    } else {
      output.push(token);
    }
  }
  while (!ops.empty()) {
    std::string topOp = ops.drop();
    if (topOp == "(" || topOp == ")") {
      throw std::runtime_error("Mismatched parentheses");
    }
    output.push(topOp);
  }
  return output;
}

  long long evaluatePostfix(Queue<std::string> postfix) {
  Stack<long long> operands;

  while (!postfix.empty()) {
    std::string token = postfix.drop();

    if (isOperator(token)) {
      applyOperation(token, operands);
    } else {
      try {
        operands.push(std::stoll(token));
      } catch (...) {
        throw std::runtime_error("Invalid number: " + token);
      }
    }
  }
  if (operands.size() != 1) {
    throw std::runtime_error("Invalid expression result");
  }
  return operands.drop();
}

long long evaluateExpression(const std::string& line) {
  Queue<std::string> postfix = infixToPostfix(line);
  return evaluatePostfix(std::move(postfix));
}
}
