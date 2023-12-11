#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "grid.h"

/** Day 10: Pipe Maze */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  enum class Direction {
    kNone = 0,
    kNorth = 1 << 0,
    kEast = 1 << 1,
    kSouth = 1 << 2,
    kWest = 1 << 3,
  };

  inline Direction operator | (Direction lhs, Direction rhs) {
    using T = std::underlying_type_t<Direction>;
    return static_cast<Direction>(static_cast<T>(lhs) | static_cast<T>(rhs));
  }

  inline Direction& operator |= (Direction& lhs, Direction rhs) {
    lhs = lhs | rhs;
    return lhs; 
  }

  inline Direction operator & (Direction lhs, Direction rhs) {
    using T = std::underlying_type_t<Direction>;
    return static_cast<Direction>(static_cast<T>(lhs) & static_cast<T>(rhs));
  }

  Direction Invert(Direction d) {
    Direction r = Direction::kNone;
    if ((d & Direction::kNorth) != Direction::kNone) r |= Direction::kSouth;
    if ((d & Direction::kSouth) != Direction::kNone) r |= Direction::kNorth;
    if ((d & Direction::kEast) != Direction::kNone) r |= Direction::kWest;
    if ((d & Direction::kWest) != Direction::kNone) r |= Direction::kEast;
    return r;
  }

  struct Pipe {
    Direction direction;
    char c;
  };

  const std::unordered_map<char, Pipe> kPipes = {
    {'|', {Direction::kNorth | Direction::kSouth, '|'}},
    {'-', {Direction::kWest | Direction::kEast, '-'}},
    {'L', {Direction::kNorth | Direction::kEast, 'L'}},
    {'J', {Direction::kNorth | Direction::kWest, 'J'}},
    {'7', {Direction::kSouth | Direction::kWest, '7'}},
    {'F', {Direction::kSouth | Direction::kEast, 'F'}},
    {'S', {Direction::kNorth | Direction::kSouth | Direction::kEast | Direction::kWest, 'S'}},
  };

  class PipeGrid: public Grid<char> {
    public:
    PipeGrid(int width, int height, std::ifstream& input) : Grid(width, height, Grid::NeighborMode::kOrthoginal) {
      int y = 0;
      std::string line;
      while (std::getline(input, line)) {
        for (int x = 0; x < line.size(); x++) {
          assign({x, y}, line.at(x));
          if (line.at(x) == 'S') {
            start_ = {x, y};
          }
        }
        y += 1;
      }
    }
    const Point& start() const { return start_; }

    std::vector<std::pair<Point, char const*>> pipe_neighbors(const Point& p) const {
      std::vector<std::pair<Point, char const*>> possible_neighbors = neighbors(p);
      std::vector<std::pair<Point, char const*>> neighbors;
      std::copy_if(possible_neighbors.begin(), possible_neighbors.end(), std::back_inserter(neighbors), [](const std::pair<Point, char const*>& v) {
        return kPipes.find(*(v.second)) != kPipes.end();
      });
      return neighbors;
    }

    private:
    Point start_;
  };

  std::pair<int, int> GetGridSize(std::ifstream& input) {
    std::string line;
    int width = 0, height = 0;
    while (std::getline(input, line)) {
      ++height; 
    }
    width = line.size();
    input.clear();
    input.seekg(0);
    return {width, height};
  }

  Direction DirectionFromTo(const Point& p1, const Point& p2) {
    Point diff = p2 - p1; 
    if (diff.x == -1) {
      return Direction::kWest;
    } else if (diff.x == 1) {
      return Direction::kEast;
    } else if (diff.y == 1) {
      return Direction::kSouth; 
    } else if (diff.y == -1) {
      return Direction::kNorth;
    }
    throw std::runtime_error("Points are not within a single unit from each other.");
    return Direction::kNone;
  }

  bool DoPipesConnect(const PipeGrid& grid, const Point& p1, const Point& p2) {
    auto pipe1 = kPipes.find(*grid.at(p1));
    auto pipe2 = kPipes.find(*grid.at(p2));
    if (pipe1 == kPipes.end() || pipe2 == kPipes.end()) {
      return false;
    }

    Direction dir = DirectionFromTo(p1, p2);
    Direction p1_dir = pipe1->second.direction & dir;
    Direction p2_dir = Invert(pipe2->second.direction) & dir;
    return (p1_dir & p2_dir) != Direction::kNone;
  }

  int Part1(std::ifstream& input) {
    std::pair<int, int> size = GetGridSize(input);
    PipeGrid grid(size.first, size.second, input); 
    int distance = 0;
    Point current_point = grid.start();
    Direction dir = Direction::kNone;
    do {
      auto neighbors = grid.pipe_neighbors(current_point);
      for (auto& neighbor : neighbors) {
        if (*neighbor.second == '.') {
          continue;
        } else if (!DoPipesConnect(grid, current_point, neighbor.first)) {
          continue;
        }
        Direction dir_to_neighbor = DirectionFromTo(current_point, neighbor.first); 
        if (Invert(dir) == dir_to_neighbor) { 
          continue;
        }

        current_point = neighbor.first;
        dir = dir_to_neighbor;
        ++distance;
        break;
      }
    } while (*grid.at(current_point) != 'S');
    return distance / 2.f;
  }

  int Part2(std::ifstream& input) {
    std::pair<int, int> size = GetGridSize(input);
    PipeGrid grid(size.first, size.second, input); 
    Grid<uint8_t> loop_grid(size.first, size.second, Grid<uint8_t>::NeighborMode::kOrthoginal);
    Point current_point = grid.start();
    Direction dir = Direction::kNone;
    do {
      auto neighbors = grid.pipe_neighbors(current_point);
      for (auto& neighbor : neighbors) {
        if (*neighbor.second == '.') {
          continue;
        } else if (!DoPipesConnect(grid, current_point, neighbor.first)) {
          continue;
        }
        Direction dir_to_neighbor = DirectionFromTo(current_point, neighbor.first); 
        if (Invert(dir) == dir_to_neighbor) { 
          continue;
        }

        current_point = neighbor.first;
        dir = dir_to_neighbor;
        loop_grid.assign(current_point, 1);
        break;
      }
    } while (*grid.at(current_point) != 'S');

    // Hack spcifically for my input.  Suppose we could write some code to replace the starting
    // pipe with the piece to finish the loop, but this is easier.
    grid.assign(grid.start(), '7');

    // March from left to right.
    int enclosed_count = 0;
    bool is_inside = false;
    for (auto itr = loop_grid.begin(); itr != loop_grid.end(); ++itr) {
      if (itr.new_row()) {
        is_inside = false;
      }

      if (*itr == 1) {
        const Direction dir = Direction::kNorth; 
        if ((kPipes.find(*grid.at(itr.point()))->second.direction & dir) != Direction::kNone) {
          is_inside = !is_inside;
        }
      } else if (is_inside) {
        ++enclosed_count;
      }
    }
    return enclosed_count;
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