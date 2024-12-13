#include <fstream>
#include <iostream>
#include <unordered_map>

#include "days/days.h"

namespace {
using namespace std::chrono;

constexpr int kDefaultBenchmarkRunCount = 100;

typedef struct Day {
  std::function<int64_t(std::ifstream&)> part_1;
  std::function<int64_t(std::ifstream&)> part_2;
} Day;

const std::array<Day, 13> kDayLookUpTable {{
  {&aoc::D1P1, &aoc::D1P2},
  {&aoc::D2P1, &aoc::D2P2},
  {&aoc::D3P1, &aoc::D3P2},
  {&aoc::D4P1, &aoc::D4P2},
  {&aoc::D5P1, &aoc::D5P2},
  {&aoc::D6P1, &aoc::D6P2},
  {&aoc::D7P1, &aoc::D7P2},
  {&aoc::D8P1, &aoc::D8P2},
  {&aoc::D9P1, &aoc::D9P2},
  {&aoc::D10P1, &aoc::D10P2},
  {&aoc::D11P1, &aoc::D11P2},
  {&aoc::D12P1, &aoc::D12P2},
  {&aoc::D13P1, &aoc::D13P2},
}};


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

void RunPart(std::function<int64_t(std::ifstream&)> func, const std::string& title, std::ifstream& input, bool should_benchmark) {
  int64_t result = -1;
  double time_ms = 0;
  if (should_benchmark) {
    StopWatch stop_watch;
    for (int i = 0; i < kDefaultBenchmarkRunCount; ++i) {
      stop_watch.Start();
      result = func(input); 
      time_ms += stop_watch.Stop();
      input.clear();
      input.seekg(0);
    }
  } else {
    result = func(input);
  }
  std::cout << title << ": " << result << std::endl;

  if (should_benchmark) {
    std::cout << "Average runtime: " << time_ms / kDefaultBenchmarkRunCount << " ms" << std::endl;  
  }
  input.clear();
  input.seekg(0);
}

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

}  // namespace

int main(int argc, char* argv[]) {
  std::unordered_map<std::string, std::string> args = ParseArgs(argc, argv);
  if (!ValidateArgs(args)) {
    std::cout << "Missing arguments" << std::endl;
    return -1;
  }

  std::ifstream input_file;
  input_file.open(args.at("-i"));

  int day = std::atoi(args.at("-d").c_str());
  if (day <= 0 || day > kDayLookUpTable.size()) {
    std::cout << "Day " << day << " does not exist" << std::endl;
    return -1;
  }
  --day;

  bool should_bench_mark = args.contains("-b");
  RunPart(kDayLookUpTable[day].part_1, "Part 1", input_file, should_bench_mark);
  RunPart(kDayLookUpTable[day].part_2, "Part 2", input_file, should_bench_mark);
  return 0;
}