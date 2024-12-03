#include <fstream>
#include <iostream>
#include <unordered_map>

#include "01/d01.h"
#include "02/d02.h"
#include "03/d03.h"

namespace {
using namespace std::chrono;
class StopWatch {
  public:
  void Start() {
    start_ = high_resolution_clock::now();
  }

  double Stop() {
    duration<double, std::milli> duration = high_resolution_clock::now() - start_;
    return duration.count();
  }
  
  private:
  high_resolution_clock::time_point start_;
};
}  // namespace

namespace aoc {

std::unordered_map<std::string, std::string> ParseArgs(int argc, char* argv[]) {
  std::unordered_map<std::string, std::string> args;
  for (int i = 0; i < argc;) {
    if (i + 1 < argc && !std::string(argv[i + 1]).starts_with("-")) {
      args.emplace(std::string(argv[i]), std::string(argv[i + 1]));
      i += 2;
    } else {
      args.emplace(std::string(argv[i]), "1");
      i += 1;
    }
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

const std::array<Day, 3> kDayLookUpTable {{
  {&D1P1, &D1P2},
  {&D2P1, &D2P2},
  {&D3P1, &D3P2},
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

  bool should_bench_mark = args.contains("-b");
  StopWatch stop_watch;

  stop_watch.Start();
  int p1 = aoc::kDayLookUpTable[day].part_1(input_file); 
  double time = stop_watch.Stop();
  std::cout << "Part 1: " << p1; 
  if (should_bench_mark) {
    std::cout << " " << time << "ms"; 
  }
  std::cout << std::endl;

  input_file.clear();
  input_file.seekg(0);
  stop_watch.Start();
  int p2 = aoc::kDayLookUpTable[day].part_2(input_file); 
  time = stop_watch.Stop();
  std::cout << "Part 2: " << p2; 
  if (should_bench_mark) {
    std::cout << " " << time << "ms"; 
  }
  std::cout << std::endl;
}