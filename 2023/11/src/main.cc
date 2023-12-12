#include <fstream>
#include <iostream>
#include <unordered_set>

#include "grid.h"
#include "parse.h"

/** Day 11: Cosmic Expansion */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  Grid<char> FillGalaxy(std::ifstream& input) {
    std::pair<int, int> size = GetGridSize(input);
    Grid<char> galaxy = Grid<char>(size.first, size.second, Grid<char>::NeighborMode::kOrthoginal);
    std::string line;
    int y = 0;
    while (std::getline(input, line)) {
      for (int x = 0; x < line.size(); ++x) {
        galaxy.assign({x, y}, line.at(x));
      }
      ++y;
    }
    return galaxy;
  }

  bool RowHasGalaxy(const Grid<char>& grid, int y) {
    for (int x = 0; x < grid.width(); ++x) {
      if (*grid.at({x, y}) == '#') {
        return true;
      }
    }
    return false;
  }

  bool ColumnHasGalaxy(const Grid<char>& grid, int x) {
    for (int y = 0; y < grid.height(); ++y) {
      if (*grid.at({x, y}) == '#') {
        return true;
      }
    }
    return false;
  }

  std::vector<int> ExpandedRows(const Grid<char>& grid) {
    std::vector<int> rows;
    for (int y = 0; y < grid.height(); y++) {
      if (!RowHasGalaxy(grid, y)) {
        rows.push_back(y);
      }
    }
    return rows;
  }

  std::vector<int> ExpandedColumns(const Grid<char>& grid) {
    std::vector<int> columns;
    for (int x = 0; x < grid.width(); x++) {
      if (!ColumnHasGalaxy(grid, x)) {
        columns.push_back(x);
      }
    }
    return columns;
  }

  Point64 AdjustedPoint(const Point& p, const std::vector<int>& expanded_rows, const std::vector<int>& expanded_columns, int64_t factor = 2) {
    Point64 new_point;
    int64_t adjustment = 0;
    for (const auto& i : expanded_columns) {
      if (p.x < i) {
        break;
      }
      ++adjustment;
    }
    new_point.x = static_cast<int64_t>(p.x) + (adjustment * factor) - adjustment;
    adjustment = 0;

    for (const auto& i : expanded_rows) {
      if (p.y < i) {
        break;
      }
      ++adjustment;
    }
    new_point.y = static_cast<int64_t>(p.y) + (adjustment * factor) - adjustment;
    return new_point;
  }

  int64_t Distance(const Point64& p1, const Point64& p2) {
    int64_t x = p1.x - p2.x;
    int64_t y = p1.y - p2.y;
    if (x < 0) { x *= -1; }
    if (y < 0) { y *= -1; }
    return x + y; 
  }

  int Part1(std::ifstream& input) {
    Grid<char> universe = FillGalaxy(input);
    std::vector<int> expanded_rows = ExpandedRows(universe);
    std::vector<int> expanded_columns = ExpandedColumns(universe);
    
    std::vector<Point64> galaxies;
    for (auto itr = universe.begin(); itr != universe.end(); ++itr) {
      if (*itr == '#') {
        galaxies.push_back(AdjustedPoint(itr.point(), expanded_rows, expanded_columns));
      }
    }

    int total = 0;
    for (auto itr1 = galaxies.begin(); itr1 != galaxies.end(); ++itr1) {
      for (auto itr2 = itr1; itr2 != galaxies.end(); ++itr2) {
        if (itr1 == itr2) {
          continue;
        }
        total += Distance(*itr1, *itr2);
      }
    }
    return total;
  }

  int64_t Part2(std::ifstream& input) {
    Grid<char> universe = FillGalaxy(input);
    std::vector<int> expanded_rows = ExpandedRows(universe);
    std::vector<int> expanded_columns = ExpandedColumns(universe);
    
    std::vector<Point64> galaxies;
    for (auto itr = universe.begin(); itr != universe.end(); ++itr) {
      if (*itr == '#') {
        galaxies.push_back(AdjustedPoint(itr.point(), expanded_rows, expanded_columns, 1000000));
      }
    }

    int64_t total = 0;
    for (auto itr1 = galaxies.begin(); itr1 != galaxies.end(); ++itr1) {
      for (auto itr2 = itr1; itr2 != galaxies.end(); ++itr2) {
        if (itr1 == itr2) {
          continue;
        }
        total += Distance(*itr1, *itr2);
      }
    }
    return total;
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