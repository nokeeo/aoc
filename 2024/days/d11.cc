#include "d11.h"

#include <iostream>

#include "frequency_counter.h"
#include "parse.h"

namespace {
using namespace aoc;

std::pair<int64_t, int64_t> SplitNumber(int64_t i) {
  int length = static_cast<int>(log10(i) + 1);
  int mul = 1;
  std::pair<int64_t, int64_t> split{0, 0};
  for (int x = 0; x < length / 2.f; ++x) {
    int temp = i / 10 * 10;
    int decimal = i - temp;
    split.second += decimal * mul;
    mul *= 10;
    i /= 10;
  }

  mul = 1;
  for (int x = 0; x < length / 2.f; ++x) {
    int temp = i / 10 * 10;
    int decimal = i - temp;
    split.first += decimal * mul;
    mul *= 10;
    i /= 10;
  }
  return split;
}

FrequencyCounter<int64_t> Blink(const FrequencyCounter<int64_t>& stones) {
  FrequencyCounter<int64_t> new_stones;
  for (const auto& s : stones) {
    if (s.first == 0) {
      new_stones.Increment(1, s.second);
    } else if (static_cast<int>((log10(s.first)) + 1) % 2 == 0) {
      auto[left, right] = SplitNumber(s.first);
      new_stones.Increment(left, s.second);
      new_stones.Increment(right, s.second);
    } else {
      new_stones.Increment(s.first * 2024, s.second);
    }
  }
  return new_stones;
}

int64_t GetTotalStones(const FrequencyCounter<int64_t>& stones) {
  int64_t total = 0;
  for (const auto& s : stones) {
    total += s.second;
  }
  return total;
}
}  // namespace

namespace aoc {

int64_t D11P1(std::ifstream& input) {
  std::string line;
  std::getline(input, line);
  std::vector<int64_t> stone_line = ParseInts<int64_t>(line);
  FrequencyCounter<int64_t> stones;
  for (const auto& s : stone_line) {
    stones.Increment(s);
  }

  for (int i = 0; i < 25; ++i) {
    stones = Blink(stones);
  }
  return GetTotalStones(stones);
}

int64_t D11P2(std::ifstream& input) {
  std::string line;
  std::getline(input, line);
  std::vector<int64_t> stone_line = ParseInts<int64_t>(line);
  FrequencyCounter<int64_t> stones;
  for (const auto& s : stone_line) {
    stones.Increment(s);
  }

  for (int i = 0; i < 75; ++i) {
    stones = Blink(stones);
  }
  return GetTotalStones(stones);
}
}  // namespace aoc
