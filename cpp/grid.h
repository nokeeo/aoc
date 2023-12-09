#include <vector>

namespace aoc {
struct Point {
  int x = 0;
  int y = 0;

  bool operator==(const Point& other) const { return x == other.x && y == other.y; }
  bool operator!=(const Point& other) const { return x != other.x || y != other.y; }
  bool operator <= (const Point& other) {
    if (y != other.y) {
      return y < other.y;
    }
    return x < other.x;
  }
};

template<typename T>
class Grid {
  public:
  struct Itr {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    Itr(Grid *g, Point p) : g_(g), p_(p) {}

    reference operator*() const { return *g_->at(p_); }
    pointer operator->() { return g_->at(p_); }

    Itr& operator++() {
      if (p_.x + 1 < g_->width()) {
        p_ = {p_.x + 1, p_.y};
        new_row_ = false;
      } else {
        p_ = {0, p_.y + 1};
        new_row_ = true;
      }
      return *this;
    }
    Itr operator++(int) { Itr temp = *this; ++(*this); return temp; }

    Itr& operator--() {
      if (p_.x - 1 >= 0) {
        p_ = {p_.x - 1, p_.y};
        new_row_ = false;
      } else {
        p_ = {static_cast<int>(g_->width()) - 1, p_.y - 1};
        new_row_ = true;
      }
      return *this;
    }
    Itr operator--(int) { Itr temp = *this; --(*this); return temp; }

    bool operator==(const Itr& other) const { return g_ == other.g_ && p_ == other.p_; }
    bool operator!=(const Itr& other) const { return g_ != other.g_ || p_ != other.p_; }

    const Point& point() const { return p_; }
    bool new_row() const { return new_row_; }

    private:
    Grid* g_;
    Point p_;
    bool new_row_ = false;
  };

  Grid(int w, int h) {
    rows_.reserve(h);
    for (int i = 0; i < h; i++) {
      rows_.push_back(std::vector<T>(w));
    }
  }

  void assign(const Point& p, T v) {
    rows_[p.y][p.x] = std::move(v);
  }

  T* at(const Point& p) {
    return &rows_[p.y][p.x];
  } 

  const T* at(const Point& p) const {
    return &rows_[p.y][p.x];
  }

  size_t height() const { return rows_.size(); }
  size_t width() const {
    if (height() == 0) {
      return 0;
    }
    return rows_[0].size();
  }

  Itr begin() { return Itr(this, {0, 0}); }
  Itr end() { return Itr(this, {0, static_cast<int>(height())}); }
  Itr rbegin() { return Itr(this, {static_cast<int>(width() - 1), static_cast<int>(height() - 1)}); }
  Itr rend() { return Itr(this, {static_cast<int>(width() - 1), -1}); }

  const bool InBounds(const Point& p) const {
    return p.x >= 0 && p.x < width() && p.y >= 0 && p.y < height(); 
  }

  std::vector<std::pair<Point, const T*>> neighbors(const Point& p) const {
    std::vector<std::pair<Point, const T*>> neighbors;
    neighbors.reserve(8); 
    Point n;
    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) { 
        n.x = p.x + x;
        n.y = p.y + y;
        if (n != p && InBounds(n)) {
          neighbors.push_back({n, at(n)});
        }
      }
    }
    return neighbors;
  }

  private:
  std::vector<std::vector<T>> rows_;
};
}  // namespace aoc

template <>
struct std::hash<aoc::Point> {
  std::size_t operator()(const aoc::Point& p) const {
    using std::size_t;
    using std::hash;
    using std::string;

    return ((hash<int>()(p.x)
             ^ (hash<int>()(p.y) << 1)) >> 1);
  }
};
