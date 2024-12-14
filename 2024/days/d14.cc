#include "d14.h"

#include <iostream>
#include <unordered_set>
#include <regex>

#include "grid.h"

namespace {
using namespace aoc;

const std::regex kGuardRegex("^p=([0-9]+),([0-9]+) v=([-]{0,1}[0-9]+),([-]{0,1}[0-9]+)$");

struct Guard {
  Point p;
  Point v;
};

std::vector<Guard> ParseGuards(std::ifstream& input) {
  std::string line;
  std::vector<Guard> guards;
  std::smatch match;
  while (std::getline(input, line)) {
    if (std::regex_search(line, match, kGuardRegex)) {
      guards.push_back({
        {atoi(match[1].str().c_str()), atoi(match[2].str().c_str())},
        {atoi(match[3].str().c_str()), atoi(match[4].str().c_str())},
      });
    }
  }
  return guards;
}

inline int modulo(int a, int b) {
  const int result = a % b;
  return result >= 0 ? result : result + b;
}

int GetRegionSize(Grid<int>& floor, const Point& p, std::unordered_set<Point>& visited) {
  std::vector<Point> stack = {p};
  int size = 0;
  while (stack.size() > 0) {
    Point cur_p = stack.back();
    stack.erase(stack.end() - 1);
    if (visited.contains(cur_p) || !*floor.at(cur_p)) {
      continue;
    }

    ++size;
    std::vector<std::pair<Point, const int*>> neighbors = floor.neighbors(cur_p);
    for (const auto& n : neighbors) {
      stack.push_back(n.first);
    }
    visited.emplace(cur_p);
  }
  return size;
}
}

namespace aoc {
int64_t D14P1(std::ifstream& input) {
  std::vector<Guard> guards = ParseGuards(input);
  constexpr int kWidth = 101;//11;
  constexpr int kHeight = 103;//7; 
  constexpr int kSteps = 100;
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
  for (const auto& g : guards) {
    Point end{modulo(g.p.x + g.v.x * kSteps, kWidth), modulo(g.p.y + g.v.y * kSteps, kHeight)};
    if (end.x < kWidth / 2 && end.y < kHeight / 2) {
      ++q1;
    } else if (end.x > kWidth / 2 && end.y < kHeight / 2) {
      ++q2;
    } else if (end.x < kWidth / 2 && end.y > kHeight / 2) {
      ++q3;
    } else if (end.x > kWidth / 2 && end.y > kHeight / 2) {
      ++q4;
    }
  }
  return q1 * q2 * q3 * q4;
}

int64_t D14P2(std::ifstream& input) {
  std::vector<Guard> guards = ParseGuards(input);
  constexpr int kWidth = 101;//11;
  constexpr int kHeight = 103;//7; 
  constexpr int kMaxSteps = 10000;
  for (int i = 0; i < kMaxSteps; ++i) {
    Grid<int> floor(kWidth, kHeight);
    for (const auto& g : guards) {
      Point end{modulo(g.p.x + g.v.x * i, kWidth), modulo(g.p.y + g.v.y * i, kHeight)};
      floor.assign(end, 1);
    }

    std::unordered_set<Point> visited;
    bool print = false;
    for (auto itr = floor.begin(); itr != floor.end(); ++itr) {
      int size = GetRegionSize(floor, itr.point(), visited);
      if (size >= 50) {
        print = true;
        break;
      }
    }

    if (print) {
      std::cout << i << std::endl;
      for (auto itr = floor.begin(); itr != floor.end(); ++itr) {
        if (itr.new_row()) {
          std::cout << std::endl;
        }
        std::cout << (*itr ? 'X' : '.');
      }
      std::cout << std::endl;
      return 1;
    }
  }
  return 0;
}
}  // namespace aoc
