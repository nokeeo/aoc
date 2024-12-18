#include "d18.h"

#include <iostream>
#include <unordered_set>
#include <queue>

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;
struct TraverseState {
  Point p;
  int steps = 0;
};

Grid<char> CreateMemory(int size) {
  Grid<char> memory(size, size, Grid<char>::NeighborMode::kOrthoginal);
  for (auto itr = memory.begin(); itr != memory.end(); ++itr) {
    *itr = '.';
  }
  return memory;
}

bool DropBytes(std::ifstream& input, Grid<char>& memory, int byte_count, Point* out_p) {
  std::string line;
  for (int i = 0; i < byte_count; ++i) {
    if (!std::getline(input, line)) {
      return false;
    }
    std::vector<int> coords = ParseInts<int>(line, ',');
    Point p{coords[0], coords[1]};
    if (memory.InBounds(p)) {
      memory.assign(p, '#');
    }
    if (out_p) {
      *out_p = p;
    }
  }
  return true;
}

int GetShortestPathSteps(Grid<char>& memory) {
  Point end_point = {static_cast<int>(memory.width()) - 1, static_cast<int>(memory.height()) - 1};
  std::queue<TraverseState> queue;
  queue.push({{0, 0}, 0});
  std::unordered_set<Point> visited;
  while (!queue.empty()) {
    const TraverseState& s = queue.front();
    if (visited.contains(s.p)) {
      queue.pop();
      continue;
    }
    visited.insert(s.p);
    if (s.p == end_point) {
      return s.steps;
    }

    std::vector<std::pair<Point, const char*>> neighbors = memory.neighbors(s.p);
    for (const auto& n : neighbors) {
      if (*n.second == '.') {
        queue.push({n.first, s.steps + 1});
      }
    } 
    queue.pop();
  }
  return -1;
}
}  // namespace

namespace aoc {
int64_t D18P1(std::ifstream& input) {
  constexpr int kSize = 71;
  constexpr int kBytesToDrop = 1024;
  Grid<char> memory = CreateMemory(kSize);
  DropBytes(input, memory, kBytesToDrop, nullptr);
  return GetShortestPathSteps(memory);
}

int64_t D18P2(std::ifstream& input) {
  constexpr int kSize = 71;
  Grid<char> memory = CreateMemory(kSize);
  Point p{0, 0};
  do {
    int shortest_path_steps = GetShortestPathSteps(memory);
    if (shortest_path_steps == -1) {
      std::cout << p.x << "," << p.y << std::endl;
      return 0;
    } 
  } while(DropBytes(input, memory, 1, &p));
  return -1;
}
}  // namespace aoc
