#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "parse.h"

/** 2023 Day 6: Wait For It */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  // Could use float for Part1, but double precision is needed for part 2.
  std::array<double, 2> SolveQuad(double a, double b, double c) {
    double sqrt = sqrtl(pow(b, 2) - 4 * a * c);
    double v1 = (-b + sqrt) / (2 * a);
    double v2 = (-b - sqrt) / (2 * a);
    if (v1 < v2) {
      return { v1, v2 };
    } else {
      return { v2, v1 };
    }
  }

  bool HasFractional(double v) {
    return v - static_cast<int64_t>(v) >= std::numeric_limits<double>::epsilon();
  }

  int64_t ParseIntKerning(const std::string& line) {
    int64_t num = 0;
    int32_t base = 0;
    for (int i = line.size() - 1; i >= 0; i--) {
      if (line.at(i) == ' ') {
        continue;
      } else if (IsDigit(line.at(i))) {
        num += static_cast<int64_t>(line.at(i) - '0') * pow(10, base);
        ++base;
      } else {
        return num;
      } 
    }
    return num;
  }

  int Part1(std::ifstream& input) {
    std::string line;
    std::getline(input, line);
    std::vector<int64_t> times = ParseInts<int64_t>(line);

    std::getline(input, line);
    std::vector<int64_t> distances = ParseInts<int64_t>(line);

    int64_t total = 1;
    for (size_t i = 0; i < times.size(); ++i) {
      std::array<double, 2> win_bounds = SolveQuad(-1, times.at(i), -distances.at(i));
      int64_t low = ceil(win_bounds[0]) + (HasFractional(win_bounds[0]) ? 0 : 1);
      int64_t high = floor(win_bounds[1]) - (HasFractional(win_bounds[1]) ? 0 : 1);
      total *= high - low + 1;
    }
    return total;
  }

  int Part2(std::ifstream& input) {
    std::string line;
    std::getline(input, line);
    int64_t time = ParseIntKerning(line);

    std::getline(input, line);
    int64_t distance = ParseIntKerning(line);

    std::array<double, 2> win_bounds = SolveQuad(-1, time, -distance);
    int64_t low = ceil(win_bounds[0]) + (HasFractional(win_bounds[0]) ? 0 : 1);
    int64_t high = floor(win_bounds[1]) - (HasFractional(win_bounds[1]) ? 0 : 1);
    return high - low + 1;
  }
}  // namespace aoc

int main() {
  std::ifstream input_file;
  input_file.open(aoc::kInputFileName);
  std::cout << aoc::Part1(input_file) << std::endl;
  input_file.clear();
  input_file.seekg(0);
  std::cout << aoc::Part2(input_file) << std::endl;
  return 0;
}