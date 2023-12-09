#include <iostream>
#include <fstream>
#include <vector>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {
struct Command {
  int amount = 0;;
  int from = 0;
  int to = 0;
};

Command ParseCommand(const std::string& line) {
  Command command;
  int delim = line.find(' ');
  int end = line.find(' ', delim + 1);
  command.amount = atoi(line.substr(delim + 1, end - delim + 1).c_str()); 
  
  delim = line.find(' ', end + 1);
  end = line.find(' ', delim + 1);
  command.from = atoi(line.substr(delim + 1, end - delim + 1).c_str()) - 1; 

  delim = line.find(' ', end + 1);
  command.to = atoi(line.substr(delim).c_str()) - 1;

  return command;
}
}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

// [T]     [D]         [L]            
// [R]     [S] [G]     [P]         [H]
// [G]     [H] [W]     [R] [L]     [P]
// [W]     [G] [F] [H] [S] [M]     [L]
// [Q]     [V] [B] [J] [H] [N] [R] [N]
// [M] [R] [R] [P] [M] [T] [H] [Q] [C]
// [F] [F] [Z] [H] [S] [Z] [T] [D] [S]
// [P] [H] [P] [Q] [P] [M] [P] [F] [D]
//  1   2   3   4   5   6   7   8   9

  std::vector<std::vector<char>> stacks = {
    {'P', 'F', 'M', 'Q', 'W', 'G', 'R', 'T'},
    {'H', 'F', 'R'},
    {'P', 'Z', 'R', 'V', 'G', 'H', 'S', 'D'},
    {'Q', 'H', 'P', 'B', 'F', 'W', 'G'},
    {'P', 'S', 'M', 'J', 'H'},
    {'M', 'Z', 'T', 'H', 'S', 'R', 'P', 'L'},
    {'P', 'T', 'H', 'N', 'M', 'L'},
    {'F', 'D', 'Q', 'R'},
    {'D', 'S', 'C', 'N', 'L', 'P', 'H'}
  };

//     [D]    
// [N] [C]    
// [Z] [M] [P]
//  1   2   3 
  // std::vector<std::vector<char>> stacks = {
  //   {'Z', 'N'},
  //   {'M', 'C', 'D'},
  //   {'P'}
  // };

  std::string line;
  while (std::getline(input_file, line)) {
    aoc2022::Command command = aoc2022::ParseCommand(line);
    std::vector<char>& from_stack = stacks[command.from];
    auto start = from_stack.end() - command.amount; 
    for (auto it = start; it != from_stack.end(); it++) {
      stacks[command.to].emplace_back(*it);
    }
    from_stack.erase(start, from_stack.end());
  }

  for (const auto& stack : stacks) {
    if (stack.size() > 0) {
      std::cout << *(stack.end() - 1);
    }
  }
  std::cout << std::endl;
  return 0;
}