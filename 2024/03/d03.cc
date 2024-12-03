#include "d03.h"

#include <iostream>
#include <regex>

#include <chrono>

#define PRINT_BENCHMARK 0

namespace {
const std::regex kMulRegex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
const std::regex kMulFrontRegex("^mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
const std::regex kDoRegex("do\\(\\)");
const std::regex kDoFrontRegex("^do\\(\\)");
const std::regex kDontRegex("don't\\(\\)");
const std::regex kDontFrontRegex("^don't\\(\\)");

int GetMatchPosition(int offset, const std::smatch& match) {
  return offset + match.position() + match.length();
}

// ~10x faster than slow solution.
int D3P2Fast(std::ifstream& input) {
  int total = 0;
  std::string line;
  bool enabled = true;
  std::smatch enabled_match;
  std::smatch mul_match;
  while(std::getline(input, line)) {
    std::string::const_iterator search_itr = line.cbegin();
    int mul_pos = 0;
    int enabled_pos = 0;
    std::string::const_iterator enabled_search_itr = line.cbegin();
    // Loop through each mul()
    while (std::regex_search(search_itr, line.cend(), mul_match, kMulRegex)) {
      // Iterate over each do/don't op before the mul op and update enabled state. 
      while (std::regex_search(enabled_search_itr, line.cend(), enabled_match, enabled ? kDontRegex : kDoRegex)) {
        if (GetMatchPosition(mul_pos, mul_match) < GetMatchPosition(enabled_pos, enabled_match)) {
          break;
        }
        enabled = !enabled;
        enabled_pos = GetMatchPosition(enabled_pos, enabled_match); 
        enabled_search_itr = enabled_match.suffix().first;
      }
      if (enabled) {
        total += std::atoi(mul_match[1].str().c_str()) * std::atoi(mul_match[2].str().c_str());
      }
      search_itr = mul_match.suffix().first;
      mul_pos = GetMatchPosition(mul_pos, mul_match); 
    }

    // Update enabled state for remaining do/don't ops.
    while (std::regex_search(enabled_search_itr, line.cend(), enabled_match, enabled ? kDontRegex : kDoRegex)) {
      enabled = !enabled;
      enabled_search_itr = enabled_match.suffix().first;
    }
  }
  return total;
}

int D3P2Slow(std::ifstream& input) {
  int total = 0;
  std::string line;
  bool enabled = true;
  std::smatch mul_match;
  while(std::getline(input, line)) {
    for (auto itr = line.cbegin(); itr != line.cend(); ++itr) {
      // Not particularly effecient, but less convoluted than search the string for the next enabled
      // and mul instructions and comparing their positions.
      if (enabled && std::regex_search(itr, line.cend(), mul_match, kMulFrontRegex)) {
        total += std::atoi(mul_match[1].str().c_str()) * std::atoi(mul_match[2].str().c_str());
      } else if (std::regex_search(itr, line.cend(), mul_match, enabled ? kDontFrontRegex : kDoFrontRegex)) {
        enabled = !enabled;
      }
    }
  }
  return total;
}

}

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
  auto fast_1 = high_resolution_clock::now();
  int fast_total = D3P2Fast(input);
  auto fast_2 = high_resolution_clock::now();

  input.clear();
  input.seekg(0);
  auto slow_1 = high_resolution_clock::now();
  int slow_total = D3P2Slow(input);
  auto slow_2 = high_resolution_clock::now();

  duration<double, std::milli> fast_duration = fast_2 - fast_1;
  duration<double, std::milli> slow_duration = slow_2 - slow_1; 

#if PRINT_BENCHMARK
  std::cout << std::endl << "Fast Solution: " << fast_duration.count() << std::endl;
  std::cout << std::endl << "Slow Solution: " << slow_duration.count() << std::endl;
#endif  // PRINT_BENCHMARK

  return slow_total != fast_total ? -1 : slow_total;
}

}  // namespace aoc
