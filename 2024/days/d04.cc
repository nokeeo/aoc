#include "d04.h"

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;

constexpr std::string_view kMatch = "XMAS";

Grid<char> CreateGrid(std::ifstream& input) {
  std::pair<int, int> size = GetGridSize(input);
  Grid<char> grid(size.first, size.second);
  std::string line;
  int y = 0;
  while (std::getline(input, line)) {
    for (int x = 0; x < line.size(); ++x) {
      grid.assign({x, y}, line.at(x));
    }
    ++y;
  }
  return grid;
}

bool MatchString(const Point& start_p, const Point& vec, const Grid<char>& grid) {
  int str_pos = 0;
  for (Point p = start_p; grid.InBounds(p); p = p + vec) {
    if (*grid.at(p) != kMatch.at(str_pos)) {
      return false;
    } 
    if (++str_pos == kMatch.size()) {
      return true;
    }
  }
  return false;
}

int GetWordCount(const Point& start_p, const Point& strip_vector, const Point& char_vector, const Grid<char>& grid) {
  int count = 0;
  for (Point strip_p = start_p; grid.InBounds(strip_p); strip_p = strip_p + strip_vector) {
    for (Point char_p = strip_p; grid.InBounds(char_p); char_p = char_p + char_vector) {
      if (MatchString(char_p, char_vector, grid)) {
        ++count;
      }
    }
  }
  return count;
}

bool IsSubGrid(const Point& origin, Grid<char>& grid, Grid<char>& sub_grid) {
  if (grid.width() == 0 || grid.height() == 0 || sub_grid.width() == 0 || sub_grid.height() == 0) {
    return false;
  }

  if (origin.x + sub_grid.width() > grid.width() || origin.y + sub_grid.height() > grid.height()) {
    return false;
  }

  for (int y = 0; y < sub_grid.height(); ++y) {
    for (int x = 0; x < sub_grid.width(); ++x) {
      if (*sub_grid.at({x, y}) == '.') {
        continue;
      }

      if (*sub_grid.at({x, y}) != *grid.at({origin.x + x, origin.y + y})) {
        return false;
      }

    }
  }
  return true;
}

int GetSubGridCount(Grid<char>& puzzle, Grid<char>& sub_grid) {
  int count = 0;
  for (auto itr = puzzle.begin(); itr != puzzle.end(); itr++) {
    if (IsSubGrid(itr.point(), puzzle, sub_grid)) {
      ++count;
    }
  }
  return count;
}

Grid<char> RotateCW(const Grid<char>& grid) {
  Grid<char> new_grid(grid.height(), grid.width());
  for (int y = 0; y < grid.height(); ++y) {
    for (int x = 0; x < grid.width(); ++x) {
      int new_x = grid.width() - 1 - y;
      int new_y = x;
      new_grid.assign({new_x, new_y}, *grid.at({x, y}));
    }
  }
  return new_grid;
}
}  // namespace

namespace aoc {

int D4P1(std::ifstream& input) {
  Grid<char> puzzle = CreateGrid(input);
  int count = 0;
  // left to right.
  count += GetWordCount({0, 0}, {0, 1}, {1, 0}, puzzle);

  // right to left.
  count += GetWordCount({static_cast<int>(puzzle.width()) - 1, 0}, {0, 1}, {-1, 0}, puzzle);

  // top to bottom.
  count += GetWordCount({0, 0}, {1, 0}, {0 , 1}, puzzle);

  // bottom to top.
  count += GetWordCount({0, static_cast<int>(puzzle.height() - 1)}, {1, 0}, {0, -1}, puzzle);

  // top left diag.
  count += GetWordCount({0, 0}, {0, static_cast<int>(puzzle.height())}, {1,1}, puzzle);
  count += GetWordCount({1, 0}, {1, 0}, {1,1}, puzzle);
  count += GetWordCount({0, 1}, {0, 1}, {1, 1}, puzzle);

  // bottom right diag (reverse of top left) 
  const Point bottom_right{static_cast<int>(puzzle.width() - 1), static_cast<int>(puzzle.height() - 1)};
  count += GetWordCount(bottom_right, {0, static_cast<int>(puzzle.height())}, {-1,-1}, puzzle);
  count += GetWordCount(bottom_right + Point{0, -1}, {0, -1}, {-1,-1}, puzzle);
  count += GetWordCount(bottom_right + Point{-1, 0}, {-1, 0}, {-1, -1}, puzzle);

  // bottom left diag.
  const Point bottom_left = {0, bottom_right.y};
  count += GetWordCount(bottom_left, {0, bottom_right.y}, {1, -1}, puzzle);
  count += GetWordCount(bottom_left + Point{0, -1}, {0, -1}, {1, -1}, puzzle);
  count += GetWordCount(bottom_left + Point{1, 0}, {1, 0}, {1, -1}, puzzle);

  // top right diag (reverse of bottom left)
  const Point top_right = {bottom_right.x, 0};
  count += GetWordCount(top_right, {bottom_right.x, 0}, {-1, 1}, puzzle);
  count += GetWordCount(top_right + Point{-1, 0}, {-1, 0}, {-1, 1}, puzzle);
  count += GetWordCount(top_right + Point{0, 1}, {0, 1}, {-1, 1}, puzzle);
  return count;
}

int D4P2(std::ifstream& input) {
  Grid<char> puzzle = CreateGrid(input);

  Grid<char> mas(3, 3);
  mas.assign({0, 0}, 'M'); mas.assign({1, 0}, '.'); mas.assign({2, 0}, 'S');
  mas.assign({0, 1}, '.'); mas.assign({1, 1}, 'A'); mas.assign({2, 1}, '.');
  mas.assign({0, 2}, 'M'); mas.assign({1, 2}, '.'); mas.assign({2, 2}, 'S');

  int count = 0;
  for (int i = 0; i < 4; ++i) {
    count += GetSubGridCount(puzzle, mas);
    mas = RotateCW(mas);
  }
  return count;
}

}  // namespace aoc
