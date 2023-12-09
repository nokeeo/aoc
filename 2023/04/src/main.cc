#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

/** 2023 Day 4: Scratchcards **/

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  class Card {
    public:
    explicit Card(std::string& line) {
      int num = 0;
      int base = 0; 
      bool delim_found = false;
      bool parsing_num = false;
      for (int i = line.size() - 1; i >= 0; i--) {
        char c = line.at(i);
        if (c == '|') {
          delim_found = true;
        } else if (c == ' ') {
          if (!parsing_num) {
            continue;
          }
          if (delim_found) {
            winning_numbers_.emplace(num);
          } else {
            numbers_.emplace(num);
          }
          num = 0;
          base = 0;
          parsing_num = false;
        } else if (c == ':') {
          break;
        } else {
          num += static_cast<int>(c - '0') * std::pow(10, base);
          ++base;
          parsing_num = true;
        }
      }
    }

    int matches() const {
      int count = 0;
      for (const auto& v : winning_numbers_) {
        if (numbers_.find(v) != numbers_.end()) {
          ++count;
        }
      }
      return count;
    }

    int value() const {
      int matches = this->matches();
      if (matches == 0) {
        return 0;
      }
      return 1 << (matches - 1);
    }

    int copies() const { return copies_; }
    void set_copies(int copies) { copies_ = copies; }

    private:
    std::unordered_set<int> numbers_;
    std::unordered_set<int> winning_numbers_;
    int copies_ = 1;
  };

  int Part1(std::ifstream& input) {
    std::string line;
    int total = 0;
    while(std::getline(input, line)) {
      aoc::Card card(line);
      total += card.value(); 
    }
    return total;
  }

  int Part2(std::ifstream& input) {
    std::string line;
    int total = 0;
    std::vector<aoc::Card> cards;
    while(std::getline(input, line)) {
      cards.emplace_back(aoc::Card(line));
    }

    for (int i = 0; i < cards.size(); ++i) {
      int matches = cards.at(i).matches();
      int value = cards.at(i).copies();
      for (int j = i + 1; j < i + 1 + matches && j < cards.size(); j++) {
        cards.at(j).set_copies(cards.at(j).copies() + value);
      }
      total += value;
    }
    return total;
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