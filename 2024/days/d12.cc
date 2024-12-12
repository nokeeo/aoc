#include "d12.h"

#include <unordered_set>

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;

struct GardenDimensions {
  int perimeter;
  int area;
  int corners;
};

struct SideTraversalState {
  Point p;
  Point dir;
  bool operator==(const SideTraversalState& s) const {
    return p == s.p && dir == s.dir;
  }
};

Grid<char> ParseLand(std::ifstream& input) {
  auto[width, height] = GetGridSize(input);
  Grid<char> land(width, height, Grid<char>::NeighborMode::kOrthoginal);
  int y = 0;
  std::string line;
  while (std::getline(input, line)) {
    for (int x = 0; x < line.size(); ++x) {
      land.assign({x, y}, line.at(x));
    }
    ++y;
  }
  return land;
}
}  // namespace

template<>
struct std::hash<SideTraversalState> {
  std::size_t operator()(const SideTraversalState& s) const {
    using std::size_t;
    using std::hash;
    using std::string;

    return ((hash<Point>()(s.p)
             ^ (hash<Point>()(s.dir) << 1)) >> 1);
  }
};

namespace {
GardenDimensions CalculateGardenDimensions(const Point& p, Grid<char>& garden, std::unordered_set<Point>& visited) {
  char& id = *garden.at(p);
  std::vector<Point> stack = {p};
  GardenDimensions dimensions = {0, 0};
  while (stack.size() > 0) {
    Point cur_p = stack.back();
    stack.erase(stack.end() - 1);
    if (visited.contains(cur_p)) {
      continue;
    }

    std::vector<std::pair<Point, const char*>> neighbors = garden.neighbors(cur_p);
    int neighbors_in_garden = 0;
    for (const auto n : neighbors) {
      if (*n.second == id) {
        ++neighbors_in_garden;
        stack.push_back(n.first);
      } else {
        dimensions.perimeter += 1;
      }
    }
    dimensions.perimeter += 4 - neighbors.size();
    dimensions.area += 1;
    if (neighbors_in_garden == 0) {
      dimensions.corners += 4;
    } else if (neighbors_in_garden == 1) {
      dimensions.corners += 2;
    } else if (neighbors_in_garden == 2) {
      // Bottom left, inside corner.
      if (garden.InBounds(cur_p + Point{1, 0}) && garden.InBounds(cur_p + Point{0, -1}) && 
          *garden.at(cur_p + Point{1, 0}) == id && *garden.at(cur_p + Point{0, -1}) == id && *garden.at(cur_p + Point{1, -1}) != id) {
        dimensions.corners += 1;
      }
      // Bottom left outside corner.
      if (garden.InBounds(cur_p + Point{1, 0}) && garden.InBounds(cur_p + Point{0, -1}) && 
          *garden.at(cur_p + Point{1, 0}) == id && *garden.at(cur_p + Point{0, -1}) == id) { 
        dimensions.corners += 1;
      }
      // Top left inside corner
      if (garden.InBounds(cur_p + Point{1, 0}) && garden.InBounds(cur_p + Point{0, 1}) && 
          *garden.at(cur_p + Point{1, 0}) == id && *garden.at(cur_p + Point{0, 1}) == id && *garden.at(cur_p + Point{1, 1}) != id) {
        dimensions.corners += 1;
      }
      // Top left outside corner.
      if (garden.InBounds(cur_p + Point{1, 0}) && garden.InBounds(cur_p + Point{0, 1}) && 
          *garden.at(cur_p + Point{1, 0}) == id && *garden.at(cur_p + Point{0, 1}) == id) { 
        dimensions.corners += 1;
      }
      // Bottom right inside corner
      if (garden.InBounds(cur_p + Point{-1, 0}) && garden.InBounds(cur_p + Point{0, -1}) && 
          *garden.at(cur_p + Point{-1, 0}) == id && *garden.at(cur_p + Point{0, -1}) == id && *garden.at(cur_p + Point{-1, -1}) != id) {
        dimensions.corners += 1;
      }
      // Bottom right outside corner.
      if (garden.InBounds(cur_p + Point{-1, 0}) && garden.InBounds(cur_p + Point{0, -1}) && 
          *garden.at(cur_p + Point{-1, 0}) == id && *garden.at(cur_p + Point{0, -1}) == id) {
        dimensions.corners += 1;
      }
      // Top right inside corner
      if (garden.InBounds(cur_p + Point{-1, 0}) && garden.InBounds(cur_p + Point{0, 1}) && 
          *garden.at(cur_p + Point{-1, 0}) == id && *garden.at(cur_p + Point{0, 1}) == id && *garden.at(cur_p + Point{-1, 1}) != id) {
        dimensions.corners += 1;
      }
      // Top right outside corner.
      if (garden.InBounds(cur_p + Point{-1, 0}) && garden.InBounds(cur_p + Point{0, 1}) && 
          *garden.at(cur_p + Point{-1, 0}) == id && *garden.at(cur_p + Point{0, 1}) == id) { 
        dimensions.corners += 1;
      }
    } else if (neighbors_in_garden == 3) {
      //X
      //XX
      //X
      if (garden.InBounds(cur_p + Point{0, -1}) && garden.InBounds(cur_p + Point{0, 1}) && garden.InBounds(cur_p + Point{1, 0}) &&
          *garden.at(cur_p + Point{0, -1}) == id && *garden.at(cur_p + Point{0, 1}) == id && *garden.at(cur_p + Point{1, 0}) == id) {
        if (*garden.at(cur_p + Point{1, -1}) != id) {
          dimensions.corners += 1;
        }
        if (*garden.at(cur_p + Point{1, 1}) != id) {
          dimensions.corners += 1;
        }
      }
      
      // X
      //XX
      // X
      if (garden.InBounds(cur_p + Point{0, -1}) && garden.InBounds(cur_p + Point{0, 1}) && garden.InBounds(cur_p + Point{-1, 0}) &&
          *garden.at(cur_p + Point{0, -1}) == id && *garden.at(cur_p + Point{0, 1}) == id && *garden.at(cur_p + Point{-1, 0}) == id) {
        if (*garden.at(cur_p + Point{-1, -1}) != id) {
          dimensions.corners += 1;
        }
        if (*garden.at(cur_p + Point{-1, 1}) != id) {
          dimensions.corners += 1;
        }
      }

      // X
      //XXX
      if (garden.InBounds(cur_p + Point{-1, 0}) && garden.InBounds(cur_p + Point{1, 0}) && garden.InBounds(cur_p + Point{0, -1}) &&
          *garden.at(cur_p + Point{-1, 0}) == id && *garden.at(cur_p + Point{1, 0}) == id && *garden.at(cur_p + Point{0, -1}) == id) {
        if (*garden.at(cur_p + Point{-1, -1}) != id) {
          dimensions.corners += 1;
        }
        if (*garden.at(cur_p + Point{1, -1}) != id) {
          dimensions.corners += 1;
        }
      }

      //XXX
      // X
      if (garden.InBounds(cur_p + Point{-1, 0}) && garden.InBounds(cur_p + Point{1, 0}) && garden.InBounds(cur_p + Point{0, 1}) &&
          *garden.at(cur_p + Point{-1, 0}) == id && *garden.at(cur_p + Point{1, 0}) == id && *garden.at(cur_p + Point{0, 1}) == id) {
        if (*garden.at(cur_p + Point{-1, 1}) != id) {
          dimensions.corners += 1;
        }
        if (*garden.at(cur_p + Point{1, 1}) != id) {
          dimensions.corners += 1;
        }
      }
    } else {
      // X
      //XXX
      // X
      if (*garden.at(cur_p + Point{-1, -1}) != id) {
        dimensions.corners += 1;
      }
      if (*garden.at(cur_p + Point{1, -1}) != id) {
        dimensions.corners += 1;
      }
      if (*garden.at(cur_p + Point{-1, 1}) != id) {
        dimensions.corners += 1;
      }
      if (*garden.at(cur_p + Point{1, 1}) != id) {
        dimensions.corners += 1;
      }
    }
    visited.emplace(cur_p);
  }
  return dimensions;
}
}  // namespace

namespace aoc {
int64_t D12P1(std::ifstream& input) {
  Grid<char> land = ParseLand(input);
  std::unordered_set<Point> visited;

  int cost = 0;
  for (auto itr = land.begin(); itr != land.end(); ++itr) {
    GardenDimensions dim = CalculateGardenDimensions(itr.point(), land, visited);
    cost += dim.perimeter * dim.area;
  }
  return cost;
}

int64_t D12P2(std::ifstream& input) {
  Grid<char> land = ParseLand(input);
  std::unordered_set<Point> visited;

  int cost = 0;
  for (auto itr = land.begin(); itr != land.end(); ++itr) {
    GardenDimensions dim = CalculateGardenDimensions(itr.point(), land, visited);
    cost += dim.corners * dim.area;
  }
  return cost;
}
}  // namespace aoc
