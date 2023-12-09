#include <iostream>
#include <fstream>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {
bool AreCharsUnique(int start, int end, std::string_view str) {
  for (int i = start; i < end; i++) {
    for (int j = i + 1; j <= end; j++) {
      if (str.at(i) == str.at(j)) {
        return false;
      }
    }
  }
  return true;
}
}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  std::string line;
  std::getline(input_file, line);
  for (int i = 13; i < line.size(); i++) {
    if(aoc2022::AreCharsUnique(i - 13, i, line)) {
      std::cout << i + 1 << std::endl;
      return 0;
    }
  }
  return 0;
}