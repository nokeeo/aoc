#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "grid.h"

/** 2023 Day 3: Gear Ratios **/

namespace aoc {
  const std::string kInputFileName = "p1.txt";
  std::pair<int, int> GetGridSize(std::ifstream& file) {
    std::string line;
    if (!std::getline(file, line)) {
      return {0, 0};
    }

    int width = line.size();
    int height = 1;
    while (std::getline(file, line)) {
      width = std::max(width, static_cast<int>(line.size()));
      height++;
    }
    file.clear();
    file.seekg(0);
    return {width, height};
  }

  void FillGrid(Grid<char>& grid, std::ifstream& file) {
    int y = 0;
    std::string line;
    while (std::getline(file, line)) {
      for (int x = 0; x < line.size(); x++) {
        grid.assign({x, y}, line.at(x));
      }
      y++;
    }
  }

  bool IsNumber(char c) {
    return c >= '0' && c <= '9';
  }

  int8_t Digit(char c) {
    return c - '0';
  }

  bool NeighborsArePiece(const std::vector<std::pair<Point, const char*>>& neighbors) {
    for (const auto& neighbor: neighbors) {
      if (!IsNumber(*neighbor.second) && *neighbor.second != '.') {
        return true;
      }
    }
    return false;
  }

  
  int Part1(std::ifstream& file) {
    std::pair<int, int> grid_size = GetGridSize(file); 
    Grid<char> schematic(grid_size.first, grid_size.second);
    FillGrid(schematic, file);

    int num = 0;
    int base = 0;
    bool is_valid = false;
    int total = 0;
    for (auto itr = schematic.rbegin(); itr != schematic.rend(); itr--) {
      if (itr.new_row()) {
        if (is_valid) {
          total += num;
        }
        num = 0;
        base = 0;
        is_valid = false;
      }

      if (IsNumber(*itr)) {
        num += Digit(*itr) * pow(10, base);
        ++base;
        if (!is_valid) {
          is_valid = NeighborsArePiece(schematic.neighbors(itr.point()));
        }
      } else {
        if (is_valid) {
          total += num;
        }
        num = 0;
        base = 0;
        is_valid = false;
      }
    }

    if (is_valid) {
      total += num;
    }
    return total;
  }

  class Gear {
    public:
    explicit Gear(const Point& location) : location_(location) {}

    void AddPartNumber(int num) { parts_.push_back(num); }
    int ratio() const {
      if (parts_.size() == 2) {
        return parts_[0] * parts_[1];
      }
      return 0;
    }
    private:
    Point location_;
    std::vector<int> parts_;
  };

  class NumberBuffer {
    public:
    void AddDigit(char d) {
      num_ += Digit(d) * std::pow(10, base_);
      ++base_;
    }

    int value() const { return num_; }
    void reset() {
      num_ = 0;
      base_ = 0;
    }

    private:
    int num_ = 0;
    int base_ = 0;
  };

  void ProcessGears(NumberBuffer& buffer, std::unordered_set<Gear*>& encountered_gears) {
    for (const auto& gear : encountered_gears) {
      gear->AddPartNumber(buffer.value());
    }
    buffer.reset();
    encountered_gears.clear();
  }

  int Part2(std::ifstream& input_file) {
    std::pair<int, int> grid_size = GetGridSize(input_file); 
    Grid<char> schematic(grid_size.first, grid_size.second);
    FillGrid(schematic, input_file);

    std::unordered_map<Point, Gear> gears;

    NumberBuffer num;
    std::unordered_set<Gear*> encountered_gears;
    for (auto itr = schematic.rbegin(); itr != schematic.rend(); itr--) {
      if (itr.new_row()) {
        ProcessGears(num, encountered_gears);
      }

      if (IsNumber(*itr)) {
        num.AddDigit(*itr);
        for (const std::pair<Point, const char*>& neighbor : schematic.neighbors(itr.point())) {
          if (IsNumber(*neighbor.second) || *neighbor.second == '.') {
            continue;
          }

          Gear* gear;
          auto point_gear = gears.find(neighbor.first);
          if (point_gear != gears.end()) {
            gear = &point_gear->second;
          }
          gear = &(*gears.insert({neighbor.first, Gear(neighbor.first)}).first).second;
          encountered_gears.emplace(gear);
        }
      } else {
        ProcessGears(num, encountered_gears);
      }
    }
    ProcessGears(num, encountered_gears);

    int total = 0;
    for (const auto& gear : gears) {
      total += gear.second.ratio();
    }

    return total;
  }
}  // namespace aoc

int main() {
  std::ifstream input_file;
  input_file.open(aoc::kInputFileName);
  std::cout << aoc::Part2(input_file) << std::endl;
}