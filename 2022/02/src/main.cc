#include <iostream>
#include <fstream>
#include <map>

const std::string INPUT_FILE_NAME = "../input.txt";

struct Play {
  int value;
  Play* beats;
  Play* loses;
};

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  Play rock = {
    .value = 1,
  };

  Play paper = {
    .value = 2,
  };

  Play scissor = {
    .value = 3
  };

  rock.beats = &scissor;
  rock.loses = &paper;
  paper.beats = &rock;
  paper.loses = &scissor;
  scissor.beats = &paper;
  scissor.loses = &rock;

  std::map<char, Play*> opp_map;
  opp_map.emplace('A', &rock);
  opp_map.emplace('B', &paper);
  opp_map.emplace('C', &scissor);

  std::string line;
  int score = 0;
  while (std::getline(input_file, line)) {
    int round_score = 0;
    if (line == "") { continue; }
    if (line.size() < 3) { continue; }

    Play* opp_play = opp_map[line.at(0)];
    Play* player_play;

    if (opp_play == nullptr) { 
      std::cout << "opp NULLPTR" << std::endl;
      return 1;
    }

    char& player_code = line.at(2);
    if (player_code == 'X') {
      player_play = opp_play->beats;
    } else if (player_code == 'Y') {
      player_play = opp_play;
    } else {
      player_play = opp_play->loses;
    }
    if (player_play == nullptr) {
      std::cout << "player NULLPTR" << std::endl;
      return 1;
    }

    if (opp_play == player_play) {
      round_score += 3;
    } else if (player_play->beats == opp_play) {
      round_score += 6;
    }
    round_score += player_play->value;
    score += round_score;
  }
  std::cout << score << std::endl;
  return 0;
}