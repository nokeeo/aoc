#include <fstream>
#include <iostream>

/** Day 9: Mirage Maintenance */ 

#include "parse.h"

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  enum class ExtrapolationDirection {
    kForward,
    kBack,
  };

  int Extrapolate(const std::vector<int>& points, ExtrapolationDirection direction = ExtrapolationDirection::kForward) {
    bool zeros = true;
    for (auto& p : points) {
      if (p != 0) {
        zeros = false;
        break;
      }
    }

    if (zeros) {
      return 0;
    }

    std::vector<int> differences;
    differences.reserve(points.size() - 2);
    for (size_t i = 1; i < points.size(); ++i) {
      differences.push_back(points[i] - points[i - 1]);
    }
    int extrapolation = Extrapolate(differences, direction);
    if (direction == ExtrapolationDirection::kForward) {
      return extrapolation + points.back();
    }
    return points.front() - extrapolation;
  }

  int Part1(std::ifstream& input) {
    int total = 0;
    std::string line;
    while (std::getline(input, line)) {
      std::vector<int> points = ParseInts<int>(line);
      total += Extrapolate(points);
    }
    return total;
  }

  int Part2(std::ifstream& input) {
    int total = 0;
    std::string line;
    while (std::getline(input, line)) {
      std::vector<int> points = ParseInts<int>(line);
      total += Extrapolate(points, ExtrapolationDirection::kBack); 
    }
    return total;
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