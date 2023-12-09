#include <iostream>
#include <fstream>
#include <set>
#include <vector>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {

namespace {
template<typename T>
std::set<T> Union(const std::set<T>& s1, const std::set<T>& s2) {
  std::set<char> diff;
  const std::set<char>& small = (s1.size() < s2.size()) ? s1 : s2;
  const std::set<char>& large = (s1.size() < s2.size()) ? s2 : s1;
  for (const auto& item : small) {
    if (large.contains(item)) {
      diff.emplace(item);
    }
  }
  return diff;
}

int GetValueCode(const char& c) {
  int ascii = int(c);
  if (ascii >= 97) {
    return ascii - 96;
  }
  return ascii - 38;
}

std::set<char> LineToSet(std::string_view line) {
  std::set<char> set;
  for (int i = 0; i < line.length(); i++) {
    set.emplace(line.at(i));
  }
  return set;
}
}  // namespace
}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  std::vector<std::string> lines;
  std::string line;
  int score = 0;
  while (std::getline(input_file, line)) {
    lines.emplace_back(std::move(line));
    if (lines.size() != 3) {
      continue;
    }

    std::set<char> same_items = aoc2022::LineToSet(lines[0]);
    same_items = aoc2022::Union(same_items, aoc2022::LineToSet(lines[1]));
    same_items = aoc2022::Union(same_items, aoc2022::LineToSet(lines[2]));
    for (const auto& item : same_items) {
      score += aoc2022::GetValueCode(item);
    }
    lines.clear();
  }
  std::cout << score << std::endl;
  return 0;
}