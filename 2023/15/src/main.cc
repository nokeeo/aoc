#include <fstream>
#include <iostream>

#include "parse.h"

/** Day 15: Lens Library */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";
  constexpr int kSlotCount = 256;

  int Hash(const std::string& str) {
    int v = 0;
    for (auto& c : str) {
      v += static_cast<int>(c);
      v *= 17;
      v = v % kSlotCount;
    }
    return v;
  }

  class Hashmap {
    public:
    using KeyValue = std::pair<std::string, int>;

    Hashmap() : slots_(kSlotCount) {}

    void insert(const std::string& str, int value) {
      auto& slot = slots_[Hash(str)];
      bool inserted = false;
      for (auto& key_value : slot) {
        if (key_value.first == str) {
          key_value.second = value;
          inserted = true;
          break;
        }
      }
      
      if (!inserted) {
        slot.push_back({str, value});
      }
    }

    void erase(const std::string& str) {
      auto& slot = slots_[Hash(str)];
      std::vector<KeyValue>::iterator delete_itr = slot.end();
      for (auto itr = slot.begin(); itr != slot.end(); ++itr) {
        if ((*itr).first == str) {
          delete_itr = itr;
          break;
        }
      }
      if (delete_itr != slot.end()) {
        slot.erase(delete_itr);
      }
    }

    int power() const {
      int power = 0;
      for (int i = 0; i < slots_.size(); i++) {
        const auto& slot = slots_[i];
        for (int j = 0; j < slot.size(); j++) {
          power += (1 + i) * (j + 1) * slot[j].second;
        }
      }
      return power;
    }

    private:
    std::vector<std::vector<KeyValue>> slots_;
  };

  int Part1(std::ifstream& input) {
    int total = 0;
    std::string line;
    while (std::getline(input, line)) {
      std::vector<std::string> steps = Split(line, ',');
      for (const auto& step : steps) {
        total += Hash(step);
      }
    }
    return total;
  }

  int Part2(std::ifstream& input) {
    std::string line;
    std::getline(input, line);
    std::vector<std::string> steps = Split(line, ',');
    Hashmap map;

    for (const auto& step : steps) {
      size_t delete_index = step.find('-');
      if (delete_index != std::string::npos) {
        // Every character, but the last character.
        map.erase(step.substr(0, step.size() - 1));
        continue;
      }

      size_t insert_index = step.find('=');
      std::string key = step.substr(0, insert_index);
      int value = atoi(step.substr(insert_index + 1).c_str());
      map.insert(key, value);
    }
    return map.power();
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