#include "days.h"

#include <iostream>
#include <regex>

namespace {
const std::regex kMulRegex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
const std::regex kSymbolRegex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)|do\\(\\)|don't\\(\\)");
}  // namespace

namespace aoc {
using namespace std::chrono;

int D3P1(std::ifstream& input) {
  int total = 0;
  std::string line;
  while(std::getline(input, line)) {
    std::smatch mul_match;
    std::string::const_iterator search_itr = line.cbegin();
    while(std::regex_search(search_itr, line.cend(), mul_match, kMulRegex)) {
      total += std::atoi(mul_match[1].str().c_str()) * std::atoi(mul_match[2].str().c_str());
      search_itr = mul_match.suffix().first;
    }
  }
  return total;
}

int D3P2(std::ifstream& input) {
  int total = 0;
  std::string line;
  bool enabled = true;
  std::smatch enabled_match;
  std::smatch match;
  while(std::getline(input, line)) {
    std::string::const_iterator search_itr = line.cbegin();
    while (std::regex_search(search_itr, line.cend(), match, kSymbolRegex)) {
      if (match[0].str().starts_with("mul")) {
        if (enabled) {
          total += std::atoi(match[1].str().c_str()) * std::atoi(match[2].str().c_str());
        }
      } else if (!enabled && match[0].str() == "do()" || enabled && match[0].str() == "don't()") {
        enabled = !enabled;
      }
      search_itr = match.suffix().first; 
    }
  }
  return total;
}
}  // namespace aoc
