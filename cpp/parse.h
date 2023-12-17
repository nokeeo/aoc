#ifndef PARSE_H_ 
#define PARSE_H_
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace aoc {
  bool IsDigit(char c) {
    return c >= '0' && c <= '9';
  }

  int8_t CharToDigit(char c) {
    return static_cast<int8_t>(c - '0');
  }

  template<typename T>
  std::vector<T> ParseInts(const std::string& line, char delim = ' ') {
    std::vector<T> nums;
    T num = 0;
    T base = 0;
    bool parsing = false;
    for (int i = line.size() - 1; i >= 0; i--) {
      if (line.at(i) == delim || line.at(i) == '-') {
        if (parsing) {
          nums.emplace_back(num * (line.at(i) == '-' ? -1 : 1));
          num = 0;
          base = 0;
        }
        parsing = false;
      } else if (IsDigit(line.at(i))) {
        num += static_cast<T>(line.at(i) - '0') * pow(10, base);
        ++base;
        parsing = true;
      } else {
        break;
      } 
    }
    if (parsing) {
      nums.emplace_back(num); 
    }
    std::reverse(nums.begin(), nums.end());
    return nums;
  }

  std::vector<std::string> Split(const std::string& str, char delim) {
    std::vector<std::string> splits;
    std::stringstream ss;
    for (int i = 0; i < str.size(); ++i) {
      if (str.at(i) == delim) {
        splits.push_back(ss.str());
        ss.str(std::string());
      } else {
        ss << str.at(i);
      }
    }
    ss.seekg(0, std::ios::end);
    ss.tellg();
    if (ss.tellg() > 0) {
      splits.push_back(ss.str());
    }
    return splits;
  }

    std::pair<int, int> GetGridSize(std::ifstream& input) {
      std::string line;
      int width = 0, height = 0;
      while (std::getline(input, line)) {
        ++height; 
      }
      width = line.size();
      input.clear();
      input.seekg(0);
      return {width, height};
    }
}  // namespace aoc
#endif  // PARSE_H_
