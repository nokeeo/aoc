#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "grid.h"
#include "parse.h"

/** Day 16: The Floor Will Be Lava */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  struct LightBeam {
    Point loc;
    Point dir;

    bool operator ==(const LightBeam& other) const {
      return loc == other.loc && dir == other.dir;
    }
  };
}  // namespace aoc

namespace std {
  template<>
  struct hash<aoc::LightBeam> {
      const size_t operator()(const aoc::LightBeam& beam) const
      {
          return std::hash<aoc::Point>()(beam.loc) ^ std::hash<aoc::Point>()(beam.dir);   
      }
  };
}

namespace aoc {
  Grid<char> CreateGrid(std::ifstream& input) {
    std::pair<int, int> size = GetGridSize(input);
    Grid<char> grid(size.first, size.second, Grid<char>::NeighborMode::kOrthoginal);
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

  LightBeam NextSpaceForMirror(char c, const LightBeam& beam) {
    if (c == '\\') {
      Point dir = {beam.dir.y, beam.dir.x};
      return LightBeam{beam.loc + dir, dir};
    }
    Point dir = {-beam.dir.y, -beam.dir.x};
    return LightBeam{beam.loc + dir, dir};
  }

  bool IsSplitterOrthoginal(char c, const Point& dir) {
    if (c == '-') {
      return dir.y != 0;
    }
    return dir.x != 0;
  }

  std::vector<LightBeam> NextSpacesForSplitter(char c, const LightBeam& beam) {
    if (IsSplitterOrthoginal(c, beam.dir)) {
      Point dir = {beam.dir.y, beam.dir.x};
      return {
        {beam.loc + dir, dir},
        {beam.loc + (dir * -1), (dir * -1)},
      };
    }
    return {{beam.loc + beam.dir, beam.dir}};
  }

  std::vector<LightBeam> BeamNeighbors(const LightBeam& beam, const Grid<char>& grid) {
    std::vector<LightBeam> neighbors;
    char c = *grid.at(beam.loc);
    if (c == '.') {
      // Empty space.
      neighbors.push_back({beam.loc + beam.dir, beam.dir});
    } else if (c == '\\' || c == '/') {
      // Mirror.
      neighbors.push_back(NextSpaceForMirror(c, beam));
    } else {
      // Splitter.
      std::vector<LightBeam> next_spaces = NextSpacesForSplitter(c, beam);
      neighbors.insert(neighbors.end(), next_spaces.begin(), next_spaces.end());
    }
    std::erase_if(neighbors, [&grid](const LightBeam& beam) {
      return !grid.InBounds(beam.loc);
    });
    return neighbors;
  }

  int Part1(std::ifstream& input) {
    Grid<char> grid = CreateGrid(input);
    std::vector<LightBeam> beams = {{{0, 0}, {1, 0}}};
    std::unordered_set<LightBeam> visited;
    std::unordered_set<Point> engaged;
    while (!beams.empty()) {
      auto beam = beams.erase(beams.end() - 1);
      if (visited.find(*beam) != visited.end()) {
        continue;
      }
      engaged.insert(beam->loc);
      visited.insert(*beam);
      std::vector<LightBeam> neighbors = BeamNeighbors(*beam, grid);
      beams.insert(beams.end(), neighbors.begin(), neighbors.end());
    }
    return engaged.size();
  }

  int GetEnergizedCount(const LightBeam& beam, const Grid<char>& grid, std::unordered_set<LightBeam>& visited, std::unordered_set<Point>& engaged, Grid<std::unordered_map<Point, int>>& cache) {
    if (visited.find(beam) != visited.end()) {
      return 0;
    }

    // Tried to add a cache here, but the brute force solution seems to work.
    // The assumption was if enountering a point in which we already counted the engaged
    // tiles for a given direction of beam travel, the count would be the same.
    // auto cache_map = cache.at(beam.loc);
    // if (auto cached = cache_map->find(beam.dir); cached != cache_map->end()) {
    //   return cached->second;
    // }

    int count = (engaged.find(beam.loc) == engaged.end()) ? 1 : 0;
    visited.insert(beam);
    engaged.insert(beam.loc);
    std::vector<LightBeam> neighbors = BeamNeighbors(beam, grid);
    for (const auto& neighbor : neighbors) {
      count += GetEnergizedCount(neighbor, grid, visited, engaged, cache);
    }
    // cache_map->insert_or_assign(beam.dir, count);
    return count; 
  } 

  int Part2(std::ifstream& input) {
    Grid<char> grid = CreateGrid(input);
    Grid<std::unordered_map<Point, int>> cache(grid.width(), grid.height());
    int max = 0;

    // Columns
    for (int x = 0; x < grid.width(); ++x) {
      std::unordered_set<LightBeam> visited;
      std::unordered_set<Point> engaged;
      max = std::max(max, GetEnergizedCount({{x, 0}, {0, 1}}, grid, visited, engaged, cache));

      visited.clear();
      engaged.clear();
      max = std::max(max, GetEnergizedCount({{x, static_cast<int>(grid.height() - 1)}, {0, -1}}, grid, visited, engaged, cache));
    }

    // Rows
    for (int y = 0; y < grid.height(); ++y) {
      std::unordered_set<LightBeam> visited;
      std::unordered_set<Point> engaged;
      max = std::max(max, GetEnergizedCount({{0, y}, {1, 0}}, grid, visited, engaged, cache));

      visited.clear();
      engaged.clear();
      max = std::max(max, GetEnergizedCount({{static_cast<int>(grid.width() - 1)}, {-1, 0}}, grid, visited, engaged, cache));
    }
    return max;
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