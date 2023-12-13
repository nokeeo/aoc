#include <fstream>
#include <iostream>
#include <unordered_map>

#include "parse.h"

/** Day 12: Hot Springs */ 

// Position in string, block count, pattern index.
using CacheKey = std::tuple<int, int, int>;
constexpr CacheKey kInvalidCacheKey = {-1, -1, -1};
constexpr int kUnfoldCount = 5;

bool IsComplete(const std::string& str) {
  return str.find('?') == std::string::npos;
}

struct HashTuple { 
  template <class T1, class T2, class T3> 
  size_t operator()(const std::tuple<T1, T2, T3>& x) const { 
      return std::get<0>(x) 
              ^ std::get<1>(x) 
              ^ std::get<2>(x); 
  } 
}; 

bool IsValidSolution(const std::string& str, const std::vector<int>& pattern) {
  int continous = 0;
  int pattern_index = 0;
  for (auto& c : str) {
    if (c == '#') {
      ++continous;
    } else if (c == '.' && continous > 0) {
      if (pattern_index < pattern.size()) {
        if (continous != pattern[pattern_index]) {
          return false;
        }
      } else {
        return false;
      }
      continous = 0;
      ++pattern_index;
    }
  }

  if (continous > 0) {
    if (pattern_index < pattern.size()) {
      if (continous != pattern[pattern_index]) {
        return false;
      }
    } else {
      return false;
    }
    ++pattern_index;
  }
  return pattern_index == pattern.size();
}

CacheKey IsPossibleSolution(const std::string& str, const std::vector<int>& pattern) {
  int continous = 0;
  int pattern_index = 0;
  for (int i = 0; i < str.size(); ++i) {
    const char& c = str.at(i);
    if (c == '#') {
      ++continous;
    } else if (c == '.' && continous > 0) {
      if (pattern_index >= pattern.size()) {
        return kInvalidCacheKey;
      }
      if (continous != pattern[pattern_index]) {
        return kInvalidCacheKey;
      }
      continous = 0;
      ++pattern_index;
    } else if (c == '?') {
      if (pattern_index >= pattern.size()) {
        return {i, continous, pattern_index};
      }
      if (continous <= pattern[pattern_index]) {
        return {i, continous, pattern_index};
      }
      return kInvalidCacheKey; 
    }
  }
  return {str.size() - 1, continous, pattern_index};
}

int64_t SolutionCombinations(const std::string& str, const std::vector<int>& pattern, std::unordered_map<CacheKey, int64_t, HashTuple>& cache) {
  if (IsComplete(str)) {
    return (IsValidSolution(str, pattern)) ? 1 : 0;
  }

  // Probably don't need to prune the tree.  Instead validate the solution and if encountering a
  // '?' look up solution in cache if it exists. 
  CacheKey cache_key = IsPossibleSolution(str, pattern);
  if (cache_key == kInvalidCacheKey) {
    return 0;
  }

  auto cache_itr = cache.find(cache_key);
  if (cache_itr != cache.end()) {
    return cache_itr->second;
  } 

  size_t pos = str.find('?');
  std::string copy = str;
  copy.replace(pos, 1, ".");

  int64_t total = SolutionCombinations(copy, pattern, cache);
  copy.replace(pos, 1, "#");
  total += SolutionCombinations(copy, pattern, cache);
  cache.insert_or_assign(cache_key, total);
  return total;
}

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  int Part1(std::ifstream& input) {
    std::string line;
    int total = 0;
    while(std::getline(input, line)) {
      std::vector<std::string> pieces = Split(line, ' ');
      std::vector<int> pattern = ParseInts<int>(pieces[1], ',');
      std::unordered_map<CacheKey, int64_t, HashTuple> cache;
      total += SolutionCombinations(pieces[0], pattern, cache); 
    }
    return total;
  }

  std::string Unfold(const std::string& str, char delim) {
    std::stringstream ss;
    for (int i = 0; i < kUnfoldCount; ++i) {
      ss << str;
      if (i != kUnfoldCount - 1) {
        ss << delim;
      }
    }
    // std::cout << ss.str() << std::endl;
    return ss.str();
  }

  int64_t Part2(std::ifstream& input) {
    std::string line;
    int64_t total = 0;
    while(std::getline(input, line)) {
      std::vector<std::string> pieces = Split(line, ' ');
      std::vector<int> pattern = ParseInts<int>(Unfold(pieces[1], ','), ',');
      std::unordered_map<CacheKey, int64_t, HashTuple> cache;
      int64_t s = SolutionCombinations(Unfold(pieces[0], '?'), pattern, cache); 
      if (s < 0) {
        std::cout << s << std::endl;
      }
      total += s;
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