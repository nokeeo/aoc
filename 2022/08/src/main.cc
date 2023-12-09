#include <iostream>
#include <functional>
#include <fstream>
#include <vector>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {

struct Point {
  Point(int x, int y): x(x), y(y) {}

  Point operator +(const Point& p) const { return Point(x + p.x, y + p.y); }

  int x;
  int y;
};

class Grid {
  public:
  bool IsInBounds(const Point& p) const {
    return p.y < rows_.size() && p.x < rows_[p.y].size();
  }

  const int& GetValue(const Point& p) const {
    return rows_[p.y][p.x];
  }

  void AppendRow(const std::vector<int>& v) {
    rows_.emplace_back(std::move(v));
  }

  Point size() const {
    int y = rows_.size();
    int x = y > 0 ? rows_[0].size() : 0;
    return Point(x, y);
  }

  void TraverseDirection(const Point& start, const Point& direction, std::function<bool(int)> callback) const{
    Point current_point = start;
    while (IsInBounds(current_point)) {
      if(callback(GetValue(current_point))) {
        return;
      }
      current_point = current_point + direction;
    } 
  }

  private:
  std::vector<std::vector<int>> rows_;
};

std::vector<int> ParseLine(const std::string& line) {
  std::vector<int> items(line.length());
  for (int i = 0; i < line.length(); i++) {
    items[i] = atoi(std::string(1, line[i]).c_str());
  }
  return items;
}

bool IsVisible(const Point& p, const Grid& g) {
  int height = g.GetValue(p);
  bool visible = true;
  std::function<bool(int)> f = [&visible, &height](int value) -> bool {
    if (value >= height) {
      visible = false;
      return true;
    } 

    return false;
  };

  // left.
  Point direction = Point(0, -1); 
  g.TraverseDirection(p + direction, direction, f);
  if (visible) {
    return visible;
  }

  //right.
  direction = Point(0, 1);
  visible = true;
  g.TraverseDirection(p + direction, direction, f);
  if (visible) {
    return visible;
  }

  //up.
  direction = Point(1, 0);
  visible = true;
  g.TraverseDirection(p + direction, direction, f);
  if (visible) {
    return visible;
  }

  // down.
  direction = Point(-1, 0);
  visible = true;
  g.TraverseDirection(p + direction, direction, f);
  return visible;
}

int Part1(const Grid& g) {
  Point grid_size = g.size();
  int visible_count = 0;
  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      if (aoc2022::IsVisible(Point(x, y), g)) {
        visible_count++;
      }
    }
  }

  return visible_count;
}

int GetViewingDistance(const Point& p, const Point& direction, const Grid& g) {
  int distance = 0;
  int height = g.GetValue(p);
  g.TraverseDirection(p + direction, direction, [&distance, &height](int value) -> bool {
    distance++;
    if (value >= height) {
      return true;
    }
    return false;
  });

  return distance;
}

int Part2(const Grid& g) {
  Point grid_size = g.size();
  int max_score = 0;
  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      Point p = Point(x, y);
      int score =
        GetViewingDistance(p, Point(-1, 0), g) *  // left.
        GetViewingDistance(p, Point(1, 0), g) *  // right.
        GetViewingDistance(p, Point(0, 1), g) *  // down.
        GetViewingDistance(p, Point(0, -1), g);  // up.
      max_score = std::max(max_score, score);
    }
  }
  return max_score;
}

}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  std::string line;
  aoc2022::Grid grid;
  while(std::getline(input_file, line)) {
    grid.AppendRow(aoc2022::ParseLine(line));
  }

  std::cout << Part2(grid) << std::endl;
  return 0;
}