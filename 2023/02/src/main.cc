#include <fstream>
#include <iostream>
#include <unordered_map>
#include <regex>

/** 2023 Day2: Cube Conundrum **/

namespace aoc {
const std::string kInputFileName = "p1.txt";
const std::regex kGameRegex("^Game ([0-9]+):");
const std::regex kColorRegex("([0-9]+) ([a-z]+)([;,])*");

class Game {
  public:
  class Round {
    public:
    int red() const { return colors_.find("red")->second; }
    int green() const { return colors_.find("green")->second; }
    int blue() const { return colors_.find("blue")->second; }
    int power() const { return red() * blue() * green(); }

    bool IsValid() const {
      return red() <= 12 && green() <= 13 && blue() <= 14;
    }

    Round Max(const Round& r) const {
      Round max;
      max.colors_["red"] = std::max(red(), r.red());
      max.colors_["green"] = std::max(green(), r.green());
      max.colors_["blue"] = std::max(blue(), r.blue());
      return max;
    }

    private: 
    std::unordered_map<std::string, int> colors_ = {
      {"red", 0}, {"green", 0}, {"blue", 0},
    }; 
    friend Game;
  };

  static Game ParseGame(const std::string& line) {
    Game game;
    std::smatch match;
    if (!std::regex_search(line, match, kGameRegex)) {
      std::cout << line << std::endl;
      throw std::runtime_error("Game ID parse failed");
    }
    game.id_ = atoi(match[1].str().c_str());

    int index = match.position() + match.length();
    Round round;
    for (int i = index; i < line.size(); i += match.position() + match.length()) {
      std::string search_str = line.substr(i);
      if (!std::regex_search(search_str, match, kColorRegex)) {
        std::cout << search_str << std::endl;
        throw std::runtime_error("Failed to parse color");
      }

      round.colors_.insert_or_assign(match[2].str(), atoi(match[1].str().c_str()));
      if (match.size() >= 4 && match[3] == ";") {
        game.rounds_.push_back(std::move(round));
        round = Round();
      }
    }
    game.rounds_.push_back(std::move(round));
    return game;
  }

  bool IsValid() const {
    for (const auto& round : rounds_) {
      if (!round.IsValid()) {
        return false;
      }
    }
    return true;
  }

  int id() const { return id_; }
  const std::vector<Round>& rounds() const { return rounds_; }

  private:
  std::vector<Round> rounds_;
  int id_ = 0;
};

int Part1(std::ifstream& file) {
  int total = 0;
  std::string line;
  while(std::getline(file, line)) {
    Game game = Game::ParseGame(line);
    if (game.IsValid()) {
      total += game.id();
    }
  }
  return total;
}

int Part2(std::ifstream& file) {
  int total = 0;
  std::string line;
  while(std::getline(file, line)) {
    Game game = Game::ParseGame(line);
    Game::Round max;
    for (const auto& round : game.rounds()) {
      max = max.Max(round);
    }
    total += max.power();
  }
  return total;
}
}  // namespace aoc

int main() {
  std::ifstream input_file;
  input_file.open(aoc::kInputFileName);
  std::string line;
  std::cout << aoc::Part2(input_file) << std::endl; 
}