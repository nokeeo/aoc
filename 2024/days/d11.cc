#include "d11.h"

#include <iostream>

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

std::vector<int64_t> Blink(const std::vector<int64_t>& stones) {
  std::vector<int64_t> new_stones;
  new_stones.reserve(stones.size());
  for (const auto& s : stones) {
    if (s == 0) {
      new_stones.push_back(1);
    } else if (static_cast<int>((log10(s)) + 1) % 2 == 0) {
      auto[left, right] = SplitNumber(s);
      new_stones.push_back(left);
      new_stones.push_back(right);
    } else {
      new_stones.push_back(s * 2024);
    }
  }
  return new_stones;
}
}  // namespace

namespace aoc {

int64_t D11P1(std::ifstream& input) {
  std::string line;
  std::getline(input, line);
  std::vector<int64_t> stones = ParseInts<int64_t>(line);
  for (int i = 0; i < 75; ++i) {
    stones = Blink(stones);
    // for (const auto& v : stones) {
    //   std::cout << v << " ";
    // }
    // std::cout << std::endl;
  }
  return stones.size();
}

int64_t D11P2(std::ifstream& input) {
  return 0;
}
}  // namespace aoc
