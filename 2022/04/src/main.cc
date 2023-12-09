#include <array>
#include <iostream>
#include <fstream>
#include <vector>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {

struct Range {
  int low = 0;
  int high = 0;

  Range() = default;
  Range(int low, int high): low(low), high(high) {}

  bool Envelopes(const Range& r) const {
    return low <= r.low && high >= r.high;
  }

  bool Overlaps(const Range& r) const {
    return !(low > r.high || high < r.low);
  }
};

Range ParsePair(const std::string& s) {
  int delim = s.find('-');
  return Range(atoi(s.substr(0, delim).c_str()), atoi(s.substr(delim + 1).c_str()));
}

std::array<Range, 2> Parse(const std::string& line) {
  std::array<Range, 2> ranges;
  int pair_delim = line.find(',');
  ranges[0] = ParsePair(line.substr(0, pair_delim));
  ranges[1] = ParsePair(line.substr(pair_delim + 1));
  return ranges;
}

}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  std::string line;
  int enveolopes_count = 0;
  while (std::getline(input_file, line)) {
    std::array<aoc2022::Range, 2> ranges = aoc2022::Parse(line);

    if (ranges[0].Overlaps(ranges[1])) {
      enveolopes_count++;
    }
  }
  std::cout << enveolopes_count << std::endl;
  return 0;
}