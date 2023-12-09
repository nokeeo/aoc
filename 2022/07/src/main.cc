#include <iostream>
#include <fstream>
#include <vector>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {

struct File {
  File(std::string name, int size) : name(name), size(size) {}

  std::string name;
  int size;
};

struct Directory {
  Directory(std::string name, Directory *parent) : name(name), parent(parent) {}

  int size() const {
    if (size_ != -1) {
      return size_;
    }

    int compute_size = 0;
    compute_size += file_size();

    for (const auto& dir : sub_dirs) {
      compute_size += dir.size();
    }

    size_ = compute_size;
    if (name == "a") {
      std::cout << size_ << std::endl;
    }
    return size_;
  } 

  int file_size() const {
    int size = 0;
    for (const auto& file : files) {
      size += file.size;
    }
    return size;
  }

  std::string name;
  std::vector<File> files;
  std::vector<Directory> sub_dirs;
  Directory* parent;

  private:
  mutable int size_ = -1;
};

Directory* ParseLine(const std::string& line, Directory* dir) {
  if (line.find("$ cd ") != std::string::npos) {
    std::string name = line.substr(5);
    if (name == ".." && dir->parent != nullptr) {
      return dir->parent;
    }

    for (auto& sub_dir : dir->sub_dirs) {
      if (sub_dir.name == name) {
        return &sub_dir;
      }
    }
  } else if (line.find("$ ls") != std::string::npos) {
    // Do nothing.
  } else if (line.find("dir ") != std::string::npos) { 
    dir->sub_dirs.emplace_back(line.substr(4), dir);
  } else {
    // Assume file.
    int delim = line.find(' ');
    dir->files.emplace_back(line.substr(delim + 1), atoi(line.substr(0, delim).c_str()));
  } 

  return dir;
}

int Part1(Directory* dir) {
  int size = 0;
  if (dir->size() <= 100000) {
    size += dir->size();
  }

  for (auto& sub_dir : dir->sub_dirs) {
    size += Part1(&sub_dir);
  }
  return size;
}

void FindDirectoriesGreaterThan(int size, std::vector<Directory*>& dirs, Directory* root) {
  if (root->size() >= size) {
    dirs.push_back(root);
  } 

  for (auto& sub_dir : root->sub_dirs) {
    FindDirectoriesGreaterThan(size, dirs, &sub_dir);
  }
}

int Part2(Directory* dir) {
  int used = dir->size();
  int unused = 70000000 - used; 
  int required = 30000000 - unused;
  std::vector<Directory*> dirs;
  FindDirectoriesGreaterThan(required, dirs, dir);

  if (dirs.size() == 0) {
    return -1;
  } else if (dirs.size() == 1) {
    return dirs[0]->size();
  }

  Directory* min = dirs[0];
  for (int i = 1; i < dirs.size(); i++) {
    min = (min->size() > dirs[i]->size()) ? dirs[i] : min;
  }
  return min->size();
}

}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  std::string line;
  aoc2022::Directory root("ROOT", nullptr);
  aoc2022::Directory* current_dir = &root;
  while(std::getline(input_file, line)) {
    if (line == "$ cd /") { continue; }
    current_dir = aoc2022::ParseLine(line, current_dir);
  }
  std::cout << aoc2022::Part2(&root) << std::endl;
  return 0;
}