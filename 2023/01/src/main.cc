#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace aoc {
const std::string kInputFileName = "p1.txt";
constexpr int8_t kDigitNotFound = -1;

const std::unordered_map<std::string, int8_t> kStringToDigit = {
  {"zero", 0},
  {"one", 1},
  {"two", 2},
  {"three", 3},
  {"four", 4},
  {"five", 5},
  {"six", 6},
  {"seven", 7},
  {"eight", 8},
  {"nine", 9},
};

int8_t GetDigit(char c) {
  int8_t digit = c - '0';
  if (digit < 0 || digit > 9) {
    return kDigitNotFound;
  }
  return digit;
}

bool MatchesString(const std::string& s, const std::string& match, size_t at) {
  if (at < 0 || at + match.size() >= s.size()) {
    return false;
  }

  for (int i = at; i < at + match.size(); i++) {
    if (s.at(i) != match.at(i - at)) {
      return false;
    }
  }
  return true;
}

bool MatchesStringReverse(const std::string& s, const std::string& match, size_t at) {
  if (at < 0 || at >= s.size() || at < match.size()) {
    return false;
  }

  for (int i = at; i >= at - (match.size() - 1); i--) {
    if (s.at(i) != match.at(match.size() - (at - i) - 1)) {
      return false;
    }
  }
  return true;
}

int8_t GetDigitString(const std::string& str, size_t at, bool reverse = false) {
  int8_t digit = GetDigit(str.at(at));
  if (digit != kDigitNotFound) {
    return digit;
  }

  for (const auto& pair : kStringToDigit) {
    if (reverse) {
      if(MatchesStringReverse(str, pair.first, at)) {
        return pair.second;
      }
    } else {
      if (MatchesString(str, pair.first, at)) {
        return pair.second;
      }
    }
  }
  return kDigitNotFound;
} 

int Part1(const std::string& str) {
  int num = 0;
  for (int i = 0; i < str.size(); i++) {
    int8_t digit = GetDigit(str.at(i));
    if (digit != kDigitNotFound) {
      num += digit * 10;
      break;
    }
  }

  for (int i = str.size() -1; i >= 0; i--) {
    int8_t digit = GetDigit(str.at(i));
    if (digit != kDigitNotFound) {
      num += digit; 
      break;
    }
  }
  return num;
}

int Part2(const std::string& str) {
  int num = 0;
  for (int i = 0; i < str.size(); i++) {
    int8_t digit = GetDigitString(str, i); 
    if (digit == kDigitNotFound) {
      continue;
    }

    num += digit * 10;
    break;
  }

  for (int i = str.size() - 1; i >= 0; i--) {
    int8_t digit = GetDigitString(str, i, /*reverse=*/true);
    if (digit == kDigitNotFound) {
      continue;
    }
    num += digit;
    break;
  }
  return num;
}
}  // namespace aoc

int main() {
  std::ifstream input_file;
  input_file.open(aoc::kInputFileName);
  int total = 0;
  std::string line;
  while (std::getline(input_file, line)) {
    total += aoc::Part2(line); 
  }
  std::cout << total << std::endl;
}