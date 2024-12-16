#include "d15.h"

#include <iostream>
#include <unordered_set>

#include "grid.h"

namespace {
using namespace aoc;

struct Warehouse {
  Grid<char> floor;
  Point robot;
};

std::pair<int, int> GetGridSize(std::ifstream& input) {
  std::string line;
  int height = 0;
  int width = 0;
  while (std::getline(input, line) && line != "") {
    ++height;
    width = line.size();
  }
  input.seekg(0);
  input.clear();
  return {width, height};
}

Warehouse ParseWarehouse(std::ifstream& input, bool p2) {
  std::string line;
  auto[width, height] = GetGridSize(input);
  if (p2) {
    width *= 2; 
  }

  Grid<char> floor = Grid<char>(width, height, Grid<char>::NeighborMode::kOrthoginal);
  int y = 0;
  Point robot{0, 0};
  while (std::getline(input, line) && line != "") {
    for (int x = 0; x < line.size(); ++x) {
      int grid_x = (p2) ? x * 2 : x;
      if (p2) {
        const char& c = line.at(x);
        char c1;
        char c2;
        if (c == 'O') {
          c1 = '[';
          c2 = ']';
        } else if (c == '@') {
          c1 = c;
          c2 = '.';
        } else {
          c1 = c;
          c2 = c;
        }
        floor.assign({grid_x, y}, c1);
        floor.assign({grid_x + 1, y}, c2);
      } else {
        floor.assign({grid_x, y}, line.at(x));
      } 
      if (line.at(x) == '@') {
        robot = {grid_x, y};
      }
    }
    ++y;
  }
  return {
    std::move(floor),
    std::move(robot),
  };
}

Point GetMovementVector(char c) {
  if (c == '<') {
    return Point{-1, 0};
  } else if (c == '>') {
    return Point{1, 0};
  } else if (c == '^') {
    return Point{0, -1};
  }
  return Point{0, 1};
}

void MoveRobot(Warehouse& warehouse, const Point& dir) {
  std::vector<Point> free_spaces = {warehouse.robot + dir};
  std::vector<Point> points_to_move;
  std::unordered_set<Point> visited;
  bool can_move = true;
  while (!free_spaces.empty()) {
    Point free_space = free_spaces.front();
    free_spaces.erase(free_spaces.begin());
    if (visited.contains(free_space)) {
      continue;
    }
    points_to_move.push_back(free_space);
    visited.emplace(free_space);
    if (!warehouse.floor.InBounds(free_space)) {
      can_move = false;
      break;
    }

    const char& c = *warehouse.floor.at(free_space);
    if (c == '.') {
      // Do nothing.
    } else if (c == 'O') {
      free_spaces.push_back(free_space + dir);
    } else if (c == '#') {
      can_move = false;
      break;
    } else if (c == '[') {
      free_spaces.push_back(free_space + dir);
      if (abs(dir.x) != 1) {
        free_spaces.push_back(free_space + Point{1, 0} + dir);
      }
    } else if (c == ']') {
      free_spaces.push_back(free_space + dir);
      if (abs(dir.x) != 1) {
        free_spaces.push_back(free_space + Point{-1, 0} + dir);
      }
    } else {
      throw std::runtime_error("Unrecognized character");
    }
  }

  if (can_move) {
    Point r_dir = {-dir.x, -dir.y};
    for (auto r_itr = points_to_move.rbegin(); r_itr != points_to_move.rend(); ++r_itr) {
      char temp = *warehouse.floor.at(*r_itr);
      warehouse.floor.assign(*r_itr, *warehouse.floor.at(*r_itr + r_dir));
      warehouse.floor.assign(*r_itr + r_dir, temp);
    }
    warehouse.robot = warehouse.robot + dir;
  }
}

int GetBoxScore(Warehouse& warehouse) {
  int score = 0;
  for (auto itr = warehouse.floor.begin(); itr != warehouse.floor.end(); ++itr) {
    if (*itr != 'O' && *itr != '[') {
      continue;
    }
    score += itr.point().y * 100 + itr.point().x;
  }
  return score;
}
}  // namespace

namespace aoc {
int64_t D15P1(std::ifstream& input) {
  Warehouse warehouse = ParseWarehouse(input,/*p2=*/false);
  std::string line;
  while (std::getline(input, line)) {
    for (int i = 0; i < line.size(); ++i) {
      Point dir = GetMovementVector(line.at(i));
      MoveRobot(warehouse, dir);
    }
  }
  return GetBoxScore(warehouse);
}

int64_t D15P2(std::ifstream& input) {
  Warehouse warehouse = ParseWarehouse(input,/*p2=*/true);
  std::string line;
  while (std::getline(input, line)) {
    for (int i = 0; i < line.size(); ++i) {
      Point dir = GetMovementVector(line.at(i));
      MoveRobot(warehouse, dir);
    }
  }
  return GetBoxScore(warehouse);
}
}  // namespace aoc
