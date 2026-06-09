#include "List.h"
#include <iostream>
#include <string>
#include <limits>

using namespace nepochatova;

int main()
{
  try
  {
    List< std::pair< std::string, List< unsigned long long > > > sequences;
    std::string name;

    while (std::cin >> name) {
      List< unsigned long long > numbers;
      unsigned long long num;

      while (std::cin >> num) {
        numbers.push_back(num);
      }
      std::cin.clear();
      sequences.push_back(std::make_pair(name, std::move(numbers)));
    }

    if (sequences.empty()) {
      std::cout << "0" << "\n";
      return 0;
    }

    auto nameIt = sequences.cbegin();
    if (nameIt != sequences.cend()) {
      std::cout << nameIt->first;
      ++nameIt;
      for (; nameIt != sequences.cend(); ++nameIt) {
        std::cout << " " << nameIt->first;
      }
    }
    std::cout << "\n";

    size_t maxLen = 0;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
      if (it->second.size() > maxLen) {
        maxLen = it->second.size();
      }
    }

    List< List< unsigned long long > > transp;
    for (size_t i = 0; i < maxLen; ++i) {
      List< unsigned long long > newList;
      for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
        if (i < it->second.size()) {
          auto elemIt = it->second.cbegin();
          for (size_t j = 0; j < i; ++j) {
            ++elemIt;
          }
          newList.push_back(*elemIt);
        }
      }
      if (!newList.empty()) {
        transp.push_back(std::move(newList));
      }
    }

    for (auto it = transp.cbegin(); it != transp.cend(); ++it) {
      auto elemIt = it->cbegin();
      if (elemIt != it->cend()) {
        std::cout << *elemIt;
        ++elemIt;

        for (; elemIt != it->cend(); ++elemIt) {
          std::cout << " " << *elemIt;
        }
      }
      std::cout << "\n";
    }

    List< unsigned long long > sums;
    for (auto it = transp.cbegin(); it != transp.cend(); ++it) {
      unsigned long long total = 0;
      for (auto elemIt = it->cbegin(); elemIt != it->cend(); ++elemIt) {
        if (total > std::numeric_limits< unsigned long long >::max() - *elemIt) {
          throw std::overflow_error("Sum calculation overflow");
        }
        total += *elemIt;
      }
      sums.push_back(total);
    }

    if (sums.empty()) {
      std::cout << "0" << "\n";
    } else {
      auto sumIt = sums.cbegin();
      if (sumIt != sums.cend()) {
        std::cout << *sumIt;
        ++sumIt;
        for (; sumIt != sums.cend(); ++sumIt) {
          std::cout << " " << *sumIt;
        }
      }
      std::cout << "\n";
    }
    return 0;
  } catch (const std::exception & e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

