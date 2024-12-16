#include "d15.h"

#include <iostream>

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

Warehouse ParseWarehouse(std::ifstream& input) {
  std::string line;
  auto[width, height] = GetGridSize(input);
  Grid<char> floor = Grid<char>(width, height, Grid<char>::NeighborMode::kOrthoginal);
  int y = 0;
  Point robot{0, 0};
  while (std::getline(input, line) && line != "") {
    for (int x = 0; x < line.size(); ++x) {
      floor.assign({x, y}, line.at(x));
      if (line.at(x) == '@') {
        robot = {x, y};
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
  Point free_space = warehouse.robot + dir;
  bool can_move = false;
  while (warehouse.floor.InBounds(free_space)) {
    const char& c = *warehouse.floor.at(free_space);
    if (c == '.') {
      can_move = true;
      break;
    } else if (c == 'O') {
      free_space = free_space + dir;
    } else if (c == '#') {
      break;
    } else {
      std::cout << c << free_space.x << " " << free_space.y << std::endl;
      throw std::runtime_error("Unrecognized character");
    }
  }

  if (can_move) {
    Point r_dir = {-dir.x, -dir.y};
    for (Point p = free_space; p != warehouse.robot; p = p + r_dir) {
      char temp = *warehouse.floor.at(p);
      warehouse.floor.assign(p, *warehouse.floor.at(p + r_dir));
      warehouse.floor.assign(p + r_dir, temp);
    }
    warehouse.robot = warehouse.robot + dir;
  }
}

int GetBoxScore(Warehouse& warehouse) {
  int score = 0;
  for (auto itr = warehouse.floor.begin(); itr != warehouse.floor.end(); ++itr) {
    if (*itr != 'O') {
      continue;
    }
    score += itr.point().y * 100 + itr.point().x;
  }
  return score;
}
}  // namespace

namespace aoc {
int64_t D15P1(std::ifstream& input) {
  Warehouse warehouse = ParseWarehouse(input);
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
  return 0;
}
}  // namespace aoc
