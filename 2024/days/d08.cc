#include "d08.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;
struct City {
  Grid<char> terrain;
  std::unordered_map<char, std::vector<Point>> antennas;
};

City ParseAntennas(std::ifstream& input) {
  std::string line;
  int y = 0;
  auto[width, height] = GetGridSize(input);
  Grid<char> terrain(width, height);
  std::unordered_map<char, std::vector<Point>> antennas;
  while (std::getline(input, line)) {
    for (int x = 0; x < line.size(); ++x) {
      char c = line.at(x);
      terrain.assign({x, y}, c);
      if (c == '.') {
        continue;
      }
      antennas[c].push_back({x, y});
    }
    ++y;
  }
  return {
    std::move(terrain),
    std::move(antennas),
  };
}

void FindAntinodes(const City& city, char antenna_id, std::unordered_set<Point>& antinodes) {
  const auto& antenna_group = city.antennas.find(antenna_id);
  if (antenna_group == city.antennas.end()) {
    return;
  }

  for (auto itr1 = antenna_group->second.cbegin(); itr1 != antenna_group->second.cend(); ++itr1) {
    for (auto itr2 = std::next(itr1); itr2 != antenna_group->second.cend(); ++itr2) {
      Point diff = *itr1 - *itr2;
      Point p1 = *itr1 + diff;
      if (city.terrain.InBounds(p1)) {
        antinodes.emplace(p1);
      }

      Point p2 = *itr2 - diff;
      if (city.terrain.InBounds(p2)) {
        antinodes.emplace(p2);
      }
    }
  }
}

void FindAntinodesHarmonics(const City& city, char antenna_id, std::unordered_set<Point>& antinodes) {
  const auto& antenna_group = city.antennas.find(antenna_id);
  if (antenna_group == city.antennas.end()) {
    return;
  }

  for (auto itr1 = antenna_group->second.cbegin(); itr1 != antenna_group->second.cend(); ++itr1) {
    for (auto itr2 = std::next(itr1); itr2 != antenna_group->second.cend(); ++itr2) {
      Point diff = *itr1 - *itr2;
      Point p1 = *itr1; 
      while (city.terrain.InBounds(p1)) {
        antinodes.emplace(p1);
        p1 = p1 + diff;
      }

      Point p2 = *itr2; 
      while (city.terrain.InBounds(p2)) {
        antinodes.emplace(p2);
        p2 = p2 - diff;
      }
    }
  }
}
}  // namespace

namespace aoc {

int D8P1(std::ifstream& input) {
  City city = ParseAntennas(input);
  std::unordered_set<Point> antinodes;
  for (const auto& antenna_group : city.antennas) {
    FindAntinodes(city, antenna_group.first, antinodes);
  }
  return antinodes.size();
}

int D8P2(std::ifstream& input) {
  City city = ParseAntennas(input);
  std::unordered_set<Point> antinodes;
  for (const auto& antenna_group : city.antennas) {
    FindAntinodesHarmonics(city, antenna_group.first, antinodes);
  }
  return antinodes.size();
}
}  // namespace aoc
