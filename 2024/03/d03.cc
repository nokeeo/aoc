#include "d03.h"

#include <iostream>
#include <regex>

namespace aoc {

const std::regex kMulRegex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
const std::regex kMulFrontRegex("^mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
const std::regex kDoRegex("^do\\(\\)");
const std::regex kDontRegex("^don't\\(\\)");

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
  while(std::getline(input, line)) {
    std::smatch mul_match;
    for (auto itr = line.cbegin(); itr != line.cend(); ++itr) {
      // Not particularly effecient, but less convoluted than search the string for the next enabled
      // and mul instructions and comparing their positions.
      if (enabled && std::regex_search(itr, line.cend(), mul_match, kMulRegex)) {
        total += std::atoi(mul_match[1].str().c_str()) * std::atoi(mul_match[2].str().c_str());
      } else if (std::regex_search(itr, line.cend(), mul_match, enabled ? kDontRegex : kDoRegex)) {
        enabled = !enabled;
      }
    }
  }
  return total;
}

}  // namespace aoc
