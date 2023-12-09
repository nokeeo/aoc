#include <iostream>
#include <fstream>
#include <vector>

const std::string INPUT_FILE_NAME = "p1.txt";

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  std::string line;
  std::vector<int> max_set;
  int total = 0;
  while (input_file) {
    std::getline(input_file, line);
    if (line == "") {
      max_set.push_back(total);
      total = 0;
    } else {
      total += atoi(line.c_str());
    }
  }
  if (total > 0) {
    max_set.push_back(total);
  }

  if (max_set.size() < 3) {
    std::cout << "Set size is too small" << std::endl;
    return 1;
  }
  std::sort(max_set.begin(), max_set.end());
  auto it = max_set.end();
  int final_total = *(--it);
  final_total += *(--it);
  final_total += *(--it);
  // for (auto& i : max_set) {
  //   std::cout << i << std::endl;
  // }
  std::cout << final_total << std::endl;
  return 0;
}