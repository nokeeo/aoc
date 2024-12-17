#include "d16.h"

#include <unordered_set>

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
  std::vector<Point> path;
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

std::vector<TraverseState> BestPaths(Maze& maze) {
  int lowest_cost = std::numeric_limits<int>::max();
  std::vector<TraverseState> final_states;
  Grid<std::unordered_map<Point, int>> cache(maze.tiles.width(), maze.tiles.height());
  for (auto itr = cache.begin(); itr != cache.end(); ++itr) {
    constexpr int kMax = std::numeric_limits<int>::max();
    *itr = {
      {Point{1, 0}, kMax},
      {Point{-1, 0}, kMax},
      {Point{0, 1}, kMax},
      {Point{0, -1}, kMax},
    };
  }

  std::vector<TraverseState> states = {{
    maze.start,
    {1, 0},
    0,
    {},
  }};
  while (!states.empty() && states.back().cost <= lowest_cost) {
    TraverseState state = states.back();
    states.erase(states.end() - 1);
    if ((*cache.at(state.p))[state.dir] < state.cost) {
      continue;
    }
    (*cache.at(state.p))[state.dir] = state.cost;

    if (*maze.tiles.at(state.p) == 'E') {
      lowest_cost = state.cost;
      final_states.emplace_back(std::move(state));
      continue;
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
      std::vector<Point> path = state.path;
      path.push_back(n.first);
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
  return final_states;
}
}  // namespace

namespace aoc {
int64_t D16P1(std::ifstream& input) {
  Maze maze = ParseMaze(input);
  std::vector<TraverseState> final_states = BestPaths(maze);
  return final_states.empty() ? -1 : final_states.front().cost; 
}

int64_t D16P2(std::ifstream& input) {
  Maze maze = ParseMaze(input);
  std::vector<TraverseState> final_states = BestPaths(maze);
  std::unordered_set<Point> path_points;
  for (const auto& s : final_states) {
    for (const auto& p : s.path) {
      path_points.insert(p);
    }
  }
  return path_points.size() + 1;
}
}  // namespace aoc
