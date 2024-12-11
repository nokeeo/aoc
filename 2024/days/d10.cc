#include "d10.h"

#include <unordered_set>

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;

struct Map {
  Grid<int> topo;
  std::vector<Point> trail_heads;
};

struct TraverseState {
  Point p;
  int length;
  std::unordered_set<Point> visited;
};

Map ParseMap(std::ifstream& input) {
  auto[width, height] = GetGridSize(input);
  Grid<int> map(width, height, Grid<int>::NeighborMode::kOrthoginal);
  std::string line;
  std::vector<Point> trail_heads;
  int y = 0;
  while (std::getline(input, line)) {
    for (int x = 0; x < line.size(); ++x) {
      if (line.at(x) == '.') {
        map.assign({x, y}, -1);
      } else {
        map.assign({x, y}, CharToDigit(line.at(x)));
      }
      if (line.at(x) == '0') {
        trail_heads.push_back({x, y});
      }
    }
    ++y;
  }
  return {
    std::move(map),
    std::move(trail_heads),
  };
}
}  // namespace

namespace aoc {

int64_t D10P1(std::ifstream& input) {
  Map map = ParseMap(input);
  int total = 0;
  for (const auto& head : map.trail_heads) {
    // BFS for each trailhead.
    std::vector<TraverseState> states = {{head, 1, {}}};
    std::unordered_set<Point> final_states;
    while (states.size() > 0) {
      auto state = states.front();
      if (*map.topo.at(state.p) == 9) {
        final_states.emplace(state.p);
      } else {
        state.visited.emplace(state.p);
        std::vector<std::pair<Point, const int*>> neighbors = map.topo.neighbors(state.p);
        for (const auto neighbor : neighbors) {
          if (*neighbor.second - 1 == *map.topo.at(state.p)) {
            states.push_back({
              neighbor.first,
              state.length + 1,
              state.visited,
            });
          }
        }
      }
      states.erase(states.begin());
    }
    total += final_states.size();
  }
  return total;
}

int64_t D10P2(std::ifstream& input) {
  Map map = ParseMap(input);
  std::vector<TraverseState> final_states;
  for (const auto& head : map.trail_heads) {
    std::vector<TraverseState> states = {{head, 1, {}}};
    while (states.size() > 0) {
      // BFS for each trailhead.
      auto state = states.front();
      if (*map.topo.at(state.p) == 9) {
        final_states.push_back(state);
      } else {
        state.visited.emplace(state.p);
        std::vector<std::pair<Point, const int*>> neighbors = map.topo.neighbors(state.p);
        for (const auto neighbor : neighbors) {
          if (*neighbor.second - 1 == *map.topo.at(state.p)) {
            states.push_back({
              neighbor.first,
              state.length + 1,
              state.visited,
            });
          }
        }
      }
      states.erase(states.begin());
    }
  }
  return final_states.size();
}
}  // namespace aoc
