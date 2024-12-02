#include <fstream>
#include <iostream>
#include <vector>

#include "parse.h"

namespace aoc {
const std::string kInputFileName = "p1.txt";

static bool IsSafe(const std::vector<int>& nums, int* out_index) {
  int direction = 0;
  for (int i = 1; i < nums.size(); ++i) {
    int diff = nums[i] - nums[i - 1];
    if (i == 1) {
      direction = diff == 0 ? 0 : diff / std::abs(diff);
    }
    if (diff == 0 || std::abs(diff) > 3 || diff / std::abs(diff) != direction) {
      if (out_index) {
        *out_index = i;
      }
      return false;
    }
  }
  return true;
}

static int Part1(std::ifstream& input) {
  std::string line;
  int safe_count = 0;
  while (std::getline(input, line)) {
    std::vector<int> nums = ParseInts<int>(line);  
    if (IsSafe(nums, NULL)) {
      ++safe_count;
    }
  }
  return safe_count;
}

static int Part2(std::ifstream& input) {
  std::string line;
  int safe_count = 0;
  int line_count = 0;
  while (std::getline(input, line)) {
    line_count++;
    std::vector<int> nums = ParseInts<int>(line); 
    int index = -1;
    bool is_safe = IsSafe(nums, &index); 
    if (!is_safe) {
      // Brute force.
      for (int i = 0; i < nums.size(); ++i) {
        int v = nums[i];
        nums.erase(nums.begin() + i);
        is_safe = IsSafe(nums, NULL);
        if (is_safe) {
          break;
        }
        nums.insert(nums.begin() + i, v);
      }
    }
    
    if (is_safe) {
      ++safe_count;
    }
  }
  return safe_count;
}

}  // namespace aoc

int main() {
  std::ifstream input_file;
  input_file.open(aoc::kInputFileName);
  std::cout << "Part 1: " << aoc::Part1(input_file) << std::endl;
  input_file.clear();
  input_file.seekg(0);
  std::cout << "Part 2: " << aoc::Part2(input_file) << std::endl;
}