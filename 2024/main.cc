#include <fstream>
#include <iostream>
#include <unordered_map>

#include "01/d01.h"
#include "02/d02.h"

namespace aoc {

std::unordered_map<std::string, std::string> ParseArgs(int argc, char* argv[]) {
  std::unordered_map<std::string, std::string> args;
  for (int i = 1; i + 1 < argc; i += 2) {
    args.emplace(std::string(argv[i]), std::string(argv[i + 1]));
  }
  return args;
}

bool ValidateArgs(const std::unordered_map<std::string, std::string>& args) {
  if (!args.contains("-i")) {
    return false;
  }
  if (!args.contains("-d")) {
    return false;
  }
  return true;
}

typedef struct Day {
  std::function<int(std::ifstream&)> part_1;
  std::function<int(std::ifstream&)> part_2;
} Day;

const std::array<Day, 2> kDayLookUpTable {{
  {&D1P1, &D1P2},
  {&D2P1, &D2P2},
}};

} // namespace aoc

int main(int argc, char* argv[]) {
  std::unordered_map<std::string, std::string> args = aoc::ParseArgs(argc, argv);
  if (!aoc::ValidateArgs(args)) {
    std::cout << "Missing arguments" << std::endl;
    return -1;
  }

  std::ifstream input_file;
  input_file.open(args.at("-i"));

  int day = std::atoi(args.at("-d").c_str());
  if (day <= 0 || day > aoc::kDayLookUpTable.size()) {
    std::cout << "Day " << day << " does not exist" << std::endl;
    return -1;
  }
  --day;

  std::cout << "Part 1: " << aoc::kDayLookUpTable[day].part_1(input_file) << std::endl;
  input_file.clear();
  input_file.seekg(0);
  std::cout << "Part 2: " << aoc::kDayLookUpTable[day].part_2(input_file) << std::endl;
}