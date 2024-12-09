#include "d09.h"

#include <unordered_set>

#include "parse.h"

namespace {
using namespace aoc;
std::vector<int> ParseDisk(const std::string& fmt) {
  std::vector<int> disk;
  bool is_file = true;
  int file_id = 0;
  for (int i = 0; i < fmt.size(); ++i) {
    int v = -1;
    int length = CharToDigit(fmt.at(i)); 
    if (is_file) {
      v = file_id;
      ++file_id;
    }
    for (int j = 0 ; j < length; ++j) {
      disk.push_back(v);
    }
    is_file = !is_file;
  }
  return disk;
}

int FreeLength(const std::vector<int>& disk, int i) {
  int length = 0;
  for (; i < disk.size() && disk[i] == -1; ++i) {
    ++length;
  }
  return length;
}

int BlockLength(const std::vector<int>& disk, int i) {
  int length = 0;
  int v = disk[i];
  if (v == -1) {
    return 0;
  }
  for (; i >= 0 && disk[i] == v; --i) {
    ++length;
  }
  return length;
}

int NextFree(std::vector<int>& disk, int i, int length = 1) {
  while (i < disk.size()) {
    for (; i < disk.size() && disk[i] != -1; ++i) {}
    int free_length = FreeLength(disk, i);
    if (free_length >= length) {
      return i;
    }
    i += free_length;
  }
  return i;
}

int NextBlock(std::vector<int>& disk, int i, int exclude = -1) {
  for (; i >= 0 && disk[i] == -1 || disk[i] == exclude; --i) {}
  return i;
}

void Fragment(std::vector<int>& disk) {
  int free = NextFree(disk, 0);
  int block = NextBlock(disk, disk.size() - 1);
  while (free < block) {
    disk[free] = disk[block];
    disk[block] = -1;
    free = NextFree(disk, ++free);
    block = NextBlock(disk, --block);
  }
}

int64_t GetChecksum(const std::vector<int>& disk) {
  int64_t checksum = 0;
  for (int i = 0; i < disk.size(); ++i) {
    if (disk[i] != -1) {
      checksum += disk[i] * i; 
    }
  }
  return checksum;
}
}  // namespace

namespace aoc {

int64_t D9P1(std::ifstream& input) {
  std::string line;
  std::getline(input, line);
  std::vector<int> disk = ParseDisk(line);
  Fragment(disk);
  return GetChecksum(disk);
}

int64_t D9P2(std::ifstream& input) {
  std::string line;
  std::getline(input, line);
  std::vector<int> disk = ParseDisk(line);
  std::unordered_set<int> moved_files;
  int block = NextBlock(disk, disk.size() - 1);
  while (block >= 0) {
    int block_length = BlockLength(disk, block);
    int free = NextFree(disk, 0, block_length);
    if (free < disk.size() && free < block && !moved_files.contains(disk[block])) {
      moved_files.emplace(disk[block]);
      for (int i = 0; i < block_length; ++i) {
        disk[i + free] = disk[block - i];
        disk[block - i] = -1;
      }
      block = NextBlock(disk, block);
    } else {
      block = NextBlock(disk, block, disk[block]);
    } 
  }
  return GetChecksum(disk);
}
}  // namespace aoc
