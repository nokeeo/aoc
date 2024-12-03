#include "d02.h"

#include <iostream>
#include <vector>

#include "parse.h"

namespace aoc {

bool IsSafe(const std::vector<int>& nums, int* out_index) {
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

int D2P1(std::ifstream& input) {
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

int D2P2(std::ifstream& input) {
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
