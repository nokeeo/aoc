#include <fstream>
#include <iostream>
#include <vector>

#include "frequency_counter.h"
#include "parse.h"

namespace aoc {
const std::string kInputFileName = "p1.txt";

int Part1(std::ifstream& input) {
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

int Part2(std::ifstream& input) {
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

int main() {
  std::ifstream input_file;
  input_file.open(aoc::kInputFileName);
  int total = 0;
  std::string line;
  std::cout << "Part 1: " << aoc::Part1(input_file) << std::endl;
  input_file.clear();
  input_file.seekg(0);
  std::cout << "Part 2: " << aoc::Part2(input_file) << std::endl;
}