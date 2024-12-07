#include "days.h"

#include <unordered_map>
#include <unordered_set>

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;
struct WallHitState {
  Point pos;
  Point dir;
  bool operator==(const WallHitState& s) const {
    return pos == s.pos && dir == s.dir; 
  }
};
}

template<>
struct std::hash<WallHitState> {
  std::size_t operator()(const WallHitState& s) const {
    using std::size_t;
    using std::hash;
    using std::string;

    return ((hash<Point>()(s.pos)
             ^ (hash<Point>()(s.dir) << 1)) >> 1);
  }
};

namespace {
struct LabState {
  Grid<char> floor;
  Point guard_pos; 
  Point guard_dir;
  std::unordered_set<WallHitState> hit_wall;

  bool IsGuardInLab() const {
    return floor.InBounds(guard_pos);
  }

  std::pair<Point, Point> GuardNextPosDir() const {
    Point dir = guard_dir;
    Point next_point = guard_pos + dir;
    if (!floor.InBounds(next_point)) {
      return {next_point, dir};
    }

    while (*floor.at(next_point) == '#') {
      dir = {
        -dir.y, dir.x
      };
      next_point = guard_pos + dir;
      if (!floor.InBounds(next_point)) {
        return {next_point, dir};
      }
    }
    return {next_point, dir};
  }

  bool MoveGuard() {
    std::pair<Point, Point> next = GuardNextPosDir();
    if (next.second != guard_dir) {
      if (!hit_wall.insert({next.first, next.second}).second) {
        return false;
      }
    }
    guard_pos = next.first;
    guard_dir = next.second;
    return true;
  }
};

bool IsStartingCharacter(char c) {
  switch (c) {
    case '<':
    case '>':
    case '^':
    case 'v':
      return true;
  }
  return false;
}

bool DoesLoop(LabState& state) {
  while (state.IsGuardInLab()) {
    if (!state.MoveGuard()) {
      return true;
    }
  }
  return false;
}

LabState ParseLab(std::ifstream& input) {
  auto [width, height] = GetGridSize(input);
  Grid<char> floor(width, height);
  Point guard_pos{0, 0};
  Point guard_dir{0, 0};
  std::string line;
  int y = 0;
  while (std::getline(input, line)) {
    for (int x = 0; x < line.size(); ++x) {
      floor.assign({x, y}, line.at(x));
      if (IsStartingCharacter(line.at(x))) {
        guard_pos = {x, y};
        if (line.at(x) == '<') {
          guard_dir = {-1, 0};
        } else if (line.at(x) == '>') {
          guard_dir = {1, 0};
        } else if (line.at(x) == '^') {
          guard_dir = {0, -1};
        } else if (line.at(x) == 'v') {
          guard_dir = {0, 1};
        }
      }
    }
    ++y;
  }
  return {
    .floor = std::move(floor),
    .guard_pos = guard_pos,
    .guard_dir = guard_dir,
  };
}
}  // namespace.


namespace aoc {

int D6P1(std::ifstream& input) {
  LabState state = ParseLab(input);
  std::unordered_set<Point> visited_points;
  while (state.IsGuardInLab()) {
    visited_points.emplace(state.guard_pos);
    state.MoveGuard();
  }
  return visited_points.size();
}

int D6P2(std::ifstream& input) {
  LabState state = ParseLab(input);
  LabState original_state = state;
  std::unordered_set<Point> obstructions;
  while (state.IsGuardInLab()) {
    Point next = state.GuardNextPosDir().first; 
    if (state.floor.InBounds(next) && *state.floor.at(next) == '.') {
      LabState copy_state = original_state;
      copy_state.hit_wall.clear();
      copy_state.floor.assign(next, '#');
      if (DoesLoop(copy_state)) {
        obstructions.emplace(next);
      }
    } 
    state.MoveGuard();
  }
  return obstructions.size(); 
}
}  // namespace aoc
