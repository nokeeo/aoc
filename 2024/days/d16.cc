#include "d16.h"

#include <unordered_set>
#include <iostream>

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;

struct Maze {
  Grid<char> tiles;
  Point start;
};

struct TraverseState {
  Point p;
  Point dir;
  int cost;
  std::unordered_set<Point> path;
};

Maze ParseMaze(std::ifstream& input) {
  auto[width, height] = GetGridSize(input);
  Grid<char> tiles(width, height, Grid<char>::NeighborMode::kOrthoginal);
  int y = 0;
  std::string line;
  Point start{0, 0};
  while (std::getline(input, line)) {
    for (int x = 0; x < line.size(); ++x) {
      tiles.assign({x, y}, line.at(x));
      if (line.at(x) == 'S') {
        start = Point{x, y};
      }
    }
    ++y;
  }
  return {
    std::move(tiles),
    std::move(start),
  };
}

}  // namespace

namespace aoc {
int64_t D16P1(std::ifstream& input) {
  Maze maze = ParseMaze(input);
  std::vector<TraverseState> states = {{
    maze.start,
    {1, 0},
    0,
    {},
  }};
  std::unordered_set<Point> visited;
  while (!states.empty()) {
    TraverseState state = states.back();
    states.erase(states.end() - 1);
    if (visited.contains(state.p)) {
      continue;
    }
    visited.emplace(state.p);

    if (*maze.tiles.at(state.p) == 'E') {
      // for (auto itr = maze.tiles.begin(); itr != maze.tiles.end(); ++itr) {
      //   if (itr.new_row()) {
      //     std::cout << std::endl;
      //   }
      //   if (state.path.contains(itr.point())) {
      //     std::cout << "0";
      //   } else {
      //     std::cout << *itr;
      //   }
      // }
      // std::cout << std::endl;
      return state.cost;
    }

    std::vector<std::pair<Point, const char*>> neighbors = maze.tiles.neighbors(state.p);
    for (const auto& n : neighbors) {
      if (*n.second == '#') {
        continue;
      }
      Point n_dir = n.first - state.p;
      Point& dir = state.dir;
      int cost_delta = 0;
      if (n_dir == dir) {
        cost_delta = 1;
      } else if (n_dir == Point{-dir.y, dir.x} || n_dir == Point{dir.y, -dir.x}) {
        cost_delta = 1000 + 1;
      } else {
        continue;
      }
      // float theta = acos((dir.x * n_dir.x + dir.y * n_dir.y) / (sqrt(pow(dir.x, 2) + pow(dir.y, 2.f)) * sqrt(pow(n_dir.x, 2) + pow(n_dir.y, 2))));
      // int cost_delta = theta < std::numeric_limits<float>::epsilon() ? 1 : (theta / (M_PI / 2.f)) * 1000;
      std::unordered_set<Point> path = state.path;
      path.emplace(n.first);
      states.push_back({
        n.first,
        n.first - state.p,
        state.cost + cost_delta,
        std::move(path),
      });
    }
    std::sort(states.begin(), states.end(), [](const TraverseState& s1, const TraverseState& s2) {
      return s1.cost > s2.cost;
    });
  }
  return -1;
}

int64_t D16P2(std::ifstream& input) {
  return 0;
}
}  // namespace aoc
