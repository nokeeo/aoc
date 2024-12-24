#include "d19.h"

#include <iostream>
#include <unordered_map>

#include "parse.h"

namespace {
using namespace aoc;

struct Shop {
  std::vector<std::string> patterns;
  std::vector<std::string> designs;
};

Shop ParseShop(std::ifstream& input) {
  std::string line;
  std::vector<std::string> final_patterns;
  while (std::getline(input, line) && line != "") {
    std::vector<std::string> patterns = Split(line, ','); 
    for (const auto& p : patterns) {
      std::vector<std::string> split = Split(p, ' ');
      if (split.size() == 1) {
        final_patterns.push_back(split[0]);
      } else {
        final_patterns.push_back(split[1]);
      }
    }
  }

  std::vector<std::string> designs;
  while (std::getline(input, line)) {
    designs.push_back(line);
  }
  std::sort(final_patterns.begin(), final_patterns.end());
  return {
    std::move(final_patterns),
    std::move(designs),
  };
}

int64_t GetValidDesignCount(const std::string& design, int index, std::vector<std::string>& patterns, std::unordered_map<std::string, int64_t>& cache) {
  std::string_view design_sub = std::string_view(design).substr(index);
  auto cache_hit = cache.find(design.substr(index));
  if (cache_hit != cache.end()) {
    return cache_hit->second;
  }

  int64_t count = 0;
  for (const auto p : patterns) {
    if (p.at(0) > design_sub.at(0)) {
      break;
    }
    if (design_sub.starts_with(p)) {
      if (design_sub.size() == p.size()) {
        ++count;
      } else {
        count += GetValidDesignCount(design, index + p.size(), patterns, cache);
      }
    }
  }
  cache.insert({std::string(design_sub), count});
  return count;
}

}  // namespace

namespace aoc {
int64_t D19P1(std::ifstream& input) {
  Shop shop = ParseShop(input);
  int valid_designs = 0;
  for (auto design : shop.designs) {
    std::unordered_map<std::string, int64_t> cache;
    if (GetValidDesignCount(design, 0, shop.patterns, cache)) {
      ++valid_designs;
    } 
  }
  return valid_designs;
}

int64_t D19P2(std::ifstream& input) {
  Shop shop = ParseShop(input);
  int64_t possible_pattern_combos = 0;
  for (auto design : shop.designs) {
    std::unordered_map<std::string, int64_t> cache;
    possible_pattern_combos += GetValidDesignCount(design, 0, shop.patterns, cache);
  }
  return possible_pattern_combos;
}
}  // namespace aoc
