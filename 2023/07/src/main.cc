#include <fstream>
#include <iostream>

#include "frequency_counter.h"
#include "parse.h"

/** Day 7: Camel Cards */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  int8_t CardToPower(char c, bool jokers = false) {
    if (IsDigit(c)) {
      return CharToDigit(c);
    }
    if (c == 'T') {
      return 10;
    }
    if (c == 'J') {
      return jokers ? 1 : 11;
    } else if (c == 'Q') {
      return 12;
    } else if (c == 'K') {
      return 13;
    }
    // Ace.
    return 14;
  }

  class Hand {
    public:
    Hand(const std::string& str, int32_t bid, bool jokers = false) : hand_(str), bid_(bid), jokers_(jokers) {
      FrequencyCounter<char> counter;
      int joker_count = 0;
      for(int i = 0; i < str.size(); ++i) {
        if (jokers && str.at(i) == 'J') {
          ++joker_count;
          continue;
        }
        counter.Increment(str.at(i));
      }

      if (joker_count > 0 && counter.unique_size() > 0) {
        auto max = counter.max();
        counter.Set(max->first, max->second + joker_count);
      } 

      if (counter.unique_size() == 1 || (counter.unique_size() == 0 && jokers)) {
        type_ = kFiveKind;
      } else if (counter.unique_size() == 5) {
        type_ = kHighCard;
      } else {
        std::vector<int32_t> counts;
        counts.reserve(counter.unique_size());
        for (const auto& itr : counter) {
          counts.push_back(itr.second);
        }
        std::sort(counts.begin(), counts.end(), std::greater<int32_t>());
        if (counter.unique_size() == 2) {
          if (counts[0] == 4) {
            type_ = kFourKind;
          } else {
            type_ = kFullHouse;
          }
        } else if (counts[0] == 3) {
          type_ = kThreeKind; 
        } else if (counts[0] == 2 && counts[1] == 2) {
          type_ = kTwoPair;
        } else if (counts[0] == 2) {
          type_ = kOnePair;
        } else {
          std::cout << str << std::endl;
          for (auto& c : counts) {
            std::cout << c << " ";
          }
          std::cout << std::endl;
          throw std::runtime_error("Failed to parse hand");
        }
      } 
    }

    bool operator==(const Hand& other) const {
      return type_ == other.type_ && hand_ == other.hand_;
    }

    bool operator<(const Hand& other) const {
      if (type_ != other.type_) {
        return type_ < other.type_; 
      }
      for (int i = 0; i < hand_.size(); ++i) {
        if (hand_.at(i) != other.hand_.at(i)) {
          return CardToPower(hand_.at(i), jokers_) < CardToPower(other.hand_.at(i), jokers_);
        }
      }
      return this < &other;
    }

    const std::string& str() const { return hand_; }
    int32_t bid() const { return bid_; }

    private:
    enum Type {
      kFiveKind = 6,
      kFourKind = 5,
      kFullHouse = 4,
      kThreeKind = 3,
      kTwoPair = 2,
      kOnePair = 1,
      kHighCard = 0,
    };
    std::string hand_;
    int32_t bid_;
    Type type_;
    bool jokers_;
  };

  int Part1(std::ifstream& input) {
    std::string line;
    std::vector<Hand> hands;
    while (std::getline(input, line)) {
      std::vector<std::string> pieces = Split(line, ' ');
      hands.emplace_back(pieces[0], atoi(pieces[1].c_str()));
    }

    std::sort(hands.begin(), hands.end());
    int32_t total = 0;
    for (size_t i = 0; i < hands.size(); i++) {
      total += (i + 1) * hands.at(i).bid();
    }
    return total;
  }

  int Part2(std::ifstream& input) {
    std::string line;
    std::vector<Hand> hands;
    while (std::getline(input, line)) {
      std::vector<std::string> pieces = Split(line, ' ');
      hands.emplace_back(pieces[0], atoi(pieces[1].c_str()), /*jokers=*/true);
    }

    std::sort(hands.begin(), hands.end());
    int32_t total = 0;
    for (size_t i = 0; i < hands.size(); i++) {
      std::cout << hands.at(i).str() << std::endl;
      total += (i + 1) * hands.at(i).bid();
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