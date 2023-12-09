#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

/** 2023 Day 5: If You Give A Seed A Fertilizer **/

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  bool IsDigit(char c) {
    return c >= '0' && c <= '9';
  }

  std::vector<uint64_t> ParseInts(const std::string& line) {
    std::vector<uint64_t> nums;
    uint64_t num = 0;
    uint64_t base = 0;
    bool parsing = false;
    for (int i = line.size() - 1; i >= 0; i--) {
      if (line.at(i) == ' ') {
        if (parsing) {
          nums.emplace_back(num);
          num = 0;
          base = 0;
        }
        parsing = false;
      } else if (IsDigit(line.at(i))) {
        num += static_cast<uint64_t>(line.at(i) - '0') * pow(10, base);
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

  class Map {
    public:
    uint64_t MapToDestination(uint64_t source) const {
      for (const auto& range : ranges_) {
        if (range.InRange(source)) {
          return range.GetDestination(source);
        }
      }
      return source;
    }

    void AddRange(uint64_t destination, uint64_t source, uint64_t length) {
      ranges_.push_back({
        .destination = destination,
        .source_start = source,
        .source_end = source + length,
      });
    }

    private:
    struct Range {
      bool InRange(uint64_t source) const {
        return source >= source_start && source < source_end; 
      }

      uint64_t GetDestination(uint64_t source) const {
        return (source - source_start) + destination;
      } 

      uint64_t destination = 0;
      uint64_t source_start = 0;
      uint64_t source_end = 0;
    };

    std::vector<Range> ranges_;
  };

  std::vector<Map> ParsePipeline(std::ifstream& input) {
    std::string line;
    std::vector<Map> pipeline;
    Map current_map;
    while (std::getline(input, line)) {
      uint64_t colon = line.find(':');
      if (colon != std::string::npos) {
        pipeline.emplace_back(std::move(current_map));
        current_map = Map(); 
      }
      std::vector<uint64_t> values = ParseInts(line);
      if (values.size() == 3) {
        current_map.AddRange(values[0], values[1], values[2]);
      }
    }
    pipeline.push_back(std::move(current_map));
    return pipeline;
  }

  uint64_t Part1(std::ifstream& input) {
    std::string line;
    std::getline(input, line);
    std::vector<uint64_t> seeds = ParseInts(line);
    std::getline(input, line);
    std::getline(input, line);
    std::vector<Map> pipeline = ParsePipeline(input); 

    uint64_t min_location = std::numeric_limits<uint64_t>().max();
    for (const auto& seed : seeds) {
      uint64_t val = seed;
      for (const auto& stage : pipeline) {
        val = stage.MapToDestination(val);
      }

      min_location = std::min(val, min_location);
    }
    return min_location; 
  }

  uint64_t Part2(std::ifstream& input) {
    std::string line;
    std::getline(input, line);
    std::vector<uint64_t> seed_ints = ParseInts(line);
    std::vector<std::pair<uint64_t, uint64_t>> seeds;
    for (int i = 0; i < seed_ints.size(); i += 2) {
      seeds.push_back({seed_ints[i], seed_ints[i + 1]});
    }

    std::getline(input, line);
    std::getline(input, line);
    std::vector<Map> pipeline = ParsePipeline(input); 

    Map p1 = pipeline.at(0);
    Map p2 = pipeline.at(1);
    Map p3 = pipeline.at(2);
    Map p4 = pipeline.at(3);
    Map p5 = pipeline.at(4);
    Map p6 = pipeline.at(5);
    Map p7 = pipeline.at(6);

    uint64_t min_location = std::numeric_limits<uint64_t>().max();
    for (const auto& seed : seeds) {
      std::cout << "Pair: " << seed.first << " " << seed.second << std::endl;
      for (uint64_t i = seed.first; i < seed.first + seed.second; ++i) {
        uint64_t val = p7.MapToDestination(p6.MapToDestination(p5.MapToDestination(p4.MapToDestination(p3.MapToDestination(p2.MapToDestination(p1.MapToDestination(i)))))));
        if (val < min_location) {
          min_location = val;
        }
      }
    }
    std::cout << "Done." << std::endl;
    return min_location; 
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