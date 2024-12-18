#include "d18.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <queue>

#include "grid.h"
#include "parse.h"

namespace {
using namespace aoc;
struct TraverseState {
  Point p;
  int steps = 0;
};

template <typename T>
class Queue {
  public:
  explicit Queue(int size) {
    data_.resize(size);
  }

  void push(const T& v) {
    data_[back_] = v;
    back_ = (back_ + 1) % data_.size();
  }

  T& front() {
    return data_[front_];
  }

  void pop() {
    front_ = (front_ + 1) % data_.size();
  }

  int size() const {
    if (front_ <= back_) {
      return back_ - front_; 
    } 
    return data_.size() - front_ + back_;
  }

  void clear() {
    front_ = 0;
    back_ = 0;
  }

  private:
  int front_ = 0;
  int back_ = 0;
  std::vector<T> data_;
};

Grid<char> CreateMemory(int size) {
  Grid<char> memory(size, size, Grid<char>::NeighborMode::kOrthoginal);
  for (auto itr = memory.begin(); itr != memory.end(); ++itr) {
    *itr = '.';
  }
  return memory;
}

bool DropBytes(std::ifstream& input, Grid<char>& memory, int byte_count, Point* out_p) {
  std::string line;
  for (int i = 0; i < byte_count; ++i) {
    if (!std::getline(input, line)) {
      return false;
    }
    std::vector<int> coords = ParseInts<int>(line, ',');
    Point p{coords[0], coords[1]};
    if (memory.InBounds(p)) {
      memory.assign(p, '#');
    }
    if (out_p) {
      *out_p = p;
    }
  }
  return true;
}

int GetShortestPathSteps(Grid<char>& memory, Queue<TraverseState>& queue) {
  Point end_point = {static_cast<int>(memory.width()) - 1, static_cast<int>(memory.height()) - 1};
  Grid<int> visited(static_cast<int>(memory.width()), static_cast<int>(memory.height()));
  queue.push({{0, 0}, 0});
  while (queue.size() > 0) {
    const TraverseState& s = queue.front();
    if (*visited.at(s.p) == 1) {
      queue.pop();
      continue;
    }
    visited.assign(s.p, 1);
    if (s.p == end_point) {
      return s.steps;
    }

    Point top = s.p + Point{0, -1};
    if (memory.InBounds(top)) {
      if (*memory.at(top) == '.') {
        queue.push({top, s.steps + 1});
      }
    }

    Point bottom = s.p + Point{0, 1};
    if (memory.InBounds(bottom)) {
      if (*memory.at(bottom) == '.') {
        queue.push({bottom, s.steps + 1});
      }
    }

    Point left = s.p + Point{-1, 0};
    if (memory.InBounds(left)) {
      if (*memory.at(left) == '.') {
        queue.push({left, s.steps + 1});
      }
    }

    Point right = s.p + Point{1, 0};
    if (memory.InBounds(right)) {
      if (*memory.at(right) == '.') {
        queue.push({right, s.steps + 1});
      }
    }
    queue.pop();
  }
  return -1;
}
}  // namespace

namespace aoc {
int64_t D18P1(std::ifstream& input) {
  constexpr int kSize = 71;
  constexpr int kBytesToDrop = 1024;
  Grid<char> memory = CreateMemory(kSize);
  DropBytes(input, memory, kBytesToDrop, nullptr);
  Queue<TraverseState> queue(500);
  return GetShortestPathSteps(memory, queue);
}

int64_t D18P2(std::ifstream& input) {
  constexpr int kSize = 71;
  Grid<char> memory = CreateMemory(kSize);
  Point p{0, 0};
  do {
    Queue<TraverseState> queue(500);
    int shortest_path_steps = GetShortestPathSteps(memory, queue);
    if (shortest_path_steps == -1) {
      std::cout << p.x << "," << p.y << std::endl;
      return 0;
    } 
  } while(DropBytes(input, memory, 1, &p));
  return -1;
}
}  // namespace aoc
