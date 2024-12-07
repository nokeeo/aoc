#include "days.h"

#include <iostream>
#include <vector>

#include "frequency_counter.h"
#include "parse.h"

namespace aoc {

int D1P1(std::ifstream& input) {
  std::string line;
  std::vector<int> left;
  std::vector<int> right;
  while (std::getline(input, line)) {
    std::vector<int> row = ParseInts<int>(line);
    left.push_back(row[0]);
    right.push_back(row[1]);
  }
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  int total_distance = 0;
  for (int i = 0; i < left.size(); ++i) {
    total_distance += std::abs(left[i] - right[i]);
  }
  return total_distance;
}

int D1P2(std::ifstream& input) {
  std::string line;
  std::vector<int> left;
  FrequencyCounter<int> right;
  
  while (std::getline(input, line)) {
    std::vector<int> row = ParseInts<int>(line);
    left.push_back(row[0]);
    right.Increment(row[1]);
  }

  int total_simalarity = 0;
  for (const auto& left_v : left) {
    total_simalarity += left_v * right.GetCount(left_v);
  }
  return total_simalarity;
}
}  // namespace aoc