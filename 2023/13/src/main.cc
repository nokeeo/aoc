#include <fstream>
#include <iostream>
#include <unordered_map>

#include "grid.h"
#include "parse.h"

/** Day 13: Point of Incidence */

namespace aoc {
  const std::string kInputFileName = "p1.txt";
  constexpr int kNotFound = -1;

  enum class MirrorEquality {
    kEqual,
    kSmudge,
    kNotEqual,
  };

  Grid<char> CreateGrid(std::ifstream& input) {
    std::vector<std::string> lines;
    std::string line;
    while(std::getline(input, line)) {
      if (line.size() == 0) {
        break;
      }
      lines.push_back(line);
    }
    if (lines.size() == 0) {
      throw std::runtime_error("Grid has not rows");
    }

    Grid<char> grid(lines.front().size(), lines.size());
    for (int y = 0; y < lines.size(); ++y) {
      for (int x = 0; x < lines[y].size(); ++x) {
        grid.assign({x, y}, lines[y].at(x));
      }
    }
    return grid;
  }

  MirrorEquality AreColumnsEqual(int c1, int c2, const Grid<char>& field) {
    bool smudge = false;
    for (int y = 0; y < field.height(); ++y) {
      if (*field.at({c1, y}) != *field.at({c2, y})) {
        if (!smudge) {
          smudge = true;
        } else {
          return MirrorEquality::kNotEqual;
        }
      }
    }
    if (smudge) {
      return MirrorEquality::kSmudge;
    }
    return MirrorEquality::kEqual;
  }

  MirrorEquality AreRowsEqual(int r1, int r2, const Grid<char>& field) {
    bool smudge = false;
    for (int x = 0; x < field.width(); ++x) {
      if (*field.at({x, r1}) != *field.at({x, r2})) {
        if (!smudge) {
          smudge = true;
        } else {
          return MirrorEquality::kNotEqual;
        }
      }
    }
    if (smudge) {
      return MirrorEquality::kSmudge;
    }
    return MirrorEquality::kEqual;
  }

  int FindVerticalMirror(const Grid<char>& field) {
    for (int x = 1; x < field.width(); ++x) {
      bool mirror = true;
      int x1 = x;
      for (int x0 = x - 1; x0 >= 0 && x1 < field.width(); --x0) {
        if (AreColumnsEqual(x0, x1, field) != MirrorEquality::kEqual) {
          mirror = false;
          break;
        }
        ++x1;
      }
      if (mirror) {
        return x;
      }
    }
    return kNotFound;
  }

  int FindHorizontalMirror(const Grid<char>& field) {
    for (int y = 1; y < field.height(); ++y) {
      bool mirror = true;
      int y1 = y;
      for (int y0 = y - 1; y0 >= 0 && y1 < field.height(); --y0) {
        if (AreRowsEqual(y0, y1, field) != MirrorEquality::kEqual) {
          mirror = false;
          break;
        }
        ++y1;
      }
      if (mirror) {
        return y;
      }
    }
    return kNotFound;
  }

  int Part1(std::ifstream& input) {
    int total = 0;
    while (!input.eof()) {
      Grid<char> field = CreateGrid(input);
      int vertical_mirror = FindVerticalMirror(field); 
      if (int vertical_mirror = FindVerticalMirror(field); vertical_mirror != kNotFound) {
        total += vertical_mirror;
      } else if (int horizontal_mirror = FindHorizontalMirror(field); horizontal_mirror != kNotFound) {
        total += horizontal_mirror * 100;
      } else {
        throw std::runtime_error("Mirror not found");
      }
    }
    return total;
  }

  int FindVerticalSmudge(const Grid<char>& field) {
    for (int x = 1; x < field.width(); ++x) {
      bool smudge = false;
      bool broken = false;
      int x1 = x;
      for (int x0 = x - 1; x0 >= 0 && x1 < field.width(); --x0) {
        MirrorEquality check = AreColumnsEqual(x0, x1, field);
        if (check == MirrorEquality::kNotEqual || (check == MirrorEquality::kSmudge && smudge)) {
          broken = true;
          break;
        } else if (check == MirrorEquality::kSmudge && !smudge) {
          smudge = true;
        } 
        ++x1;
      }
      if (!broken && smudge) {
        return x;
      }
    }
    return kNotFound;
  }

  int FindHorizontalSmudge(const Grid<char>& field) {
    for (int y = 1; y < field.height(); ++y) {
      bool smudge = false;
      bool broken = false;
      int y1 = y;
      for (int y0 = y - 1; y0 >= 0 && y1 < field.height(); --y0) {
        MirrorEquality check = AreRowsEqual(y0, y1, field);
        if (check == MirrorEquality::kNotEqual || (check == MirrorEquality::kSmudge && smudge)) {
          broken = true;
          break;
        } else if (check == MirrorEquality::kSmudge && !smudge) {
          smudge = true;
        } 
        ++y1;
      }
      if (!broken && smudge) {
        return y;
      }
    }
    return kNotFound;
  }

  int Part2(std::ifstream& input) {
    int total = 0;
    while (!input.eof()) {
      Grid<char> field = CreateGrid(input);
      if (int vertical_mirror = FindVerticalSmudge(field); vertical_mirror != kNotFound) {
        total += vertical_mirror;
      } else if (int horizontal_mirror = FindHorizontalSmudge(field); horizontal_mirror != kNotFound) {
        total += horizontal_mirror * 100;
      } else {
        throw std::runtime_error("Mirror not found");
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