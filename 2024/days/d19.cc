#include "d19.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

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

bool IsDesignValid(const std::string& design, int index, std::vector<std::string>& patterns, std::unordered_set<std::string>& failed) {
  std::string_view design_sub = std::string_view(design).substr(index);
  if (failed.contains(design.substr(index))) {
    return false;
  }
  for (const auto p : patterns) {
    if (p.at(0) > design_sub.at(0)) {
      break;
    }
    if (design_sub.starts_with(p)) {
      if (design_sub.size() == p.size()) {
        return true;
      }
      bool is_valid = IsDesignValid(design, index + p.size(), patterns, failed);
      if (is_valid) {
        return true;
      } else {
        failed.insert(std::string(design_sub));
      }
    }
  }
  return false;
}

}  // namespace

namespace aoc {
int64_t D19P1(std::ifstream& input) {
  Shop shop = ParseShop(input);
  int valid_designs = 0;
  for (auto design : shop.designs) {
    std::unordered_set<std::string> failed;
    if (IsDesignValid(design, 0, shop.patterns, failed)) {
      ++valid_designs;
    } 
  }
  return valid_designs;
}

int64_t D19P2(std::ifstream& input) {
  return 0;
}
}  // namespace aoc
