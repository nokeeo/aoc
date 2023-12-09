#include <functional>
#include <fstream>
#include <iostream>
#include <vector>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {

class Op {
  public:
  enum Options {
    kLeft = 0,
    kRight = 1,
  };

  Op(int left, int right, std::bitset<2> options): left_(left), right_(right), options_(options){}
  virtual ~Op() = default;

  virtual int operator()(int left, int right) const = 0;
  int operator()(int old_value) const {
    int left = options_.test(Options::kLeft) ? old_value : left_;
    int right = options_.test(Options::kRight) ? old_value : right_; 
    return (*this)(left, right);
  }

  private:
  std::bitset<2> options_;
  int left_ = 0;
  int right_ = 0;
};

class AddOp: public Op {
  public:
  AddOp(int left, int right, std::bitset<2> options): Op(left, right, options) {}
  int operator()(int left, int right) const {
    return left + right;
  }
};

class MultiplyOp: public Op {
  public:
  MultiplyOp(int left, int right, std::bitset<2> options): Op(left, right, options) {}
  int operator()(int left, int right) const {
    return left * right;
  }
};

std::vector<int> ParseItems(const std::string& str) {
  std::vector<int> items;
  int pos = 0;
  int delim = str.find(',');
  int last_delim = -1;
  while (delim != std::string::npos) {
    items.emplace_back(atoi(str.substr(pos, delim).c_str()));
    pos = delim + 2;
    last_delim = delim;
    delim = str.find(',', pos);
  }

  items.emplace_back(atoi(str.substr(last_delim + 1).c_str()));
  return items;
}

std::unique_ptr<Op> ParseOp(const std::string& str) {
  int start_delim = str.find("new = ") + 6;
  int op_delim = str.find(' ', start_delim);
  std::string left_str = str.substr(start_delim, op_delim - start_delim);
  std::bitset<2> options;
  int left = 0;
  if (left_str == "old") {
    options.set(Op::Options::kLeft);
  } else {
    left = atoi(left_str.c_str());
  }

  int right = 0;
  std::string right_str = str.substr(op_delim + 3);
  if (right_str == "old") {
    options.set(Op::Options::kRight);
  } else {
    right = atoi(right_str.c_str());
  } 

  if (str.at(op_delim + 1) == '+') {
    return std::make_unique<AddOp>(left, right, options);
  } else {
    return std::make_unique<MultiplyOp>(left, right, options);
  }
}

class Monkey {
  public:
  struct InspectResult {
    int value;
    int monkey_index;
  };

  static Monkey CreateMonkeyFromStream(std::ifstream& s) {
    std::string line;
    // starting items.
    std::getline(s, line);
    int delim = line.find(':');
    std::vector<int> items = ParseItems(line.substr(delim + 2));

    // operation.
    std::getline(s, line);
    delim = line.find(':');
    std::unique_ptr<Op> op = ParseOp(line.substr(delim + 2));

    // test.
    std::getline(s, line);
    delim = line.find("by");
    int test = atoi(line.substr(delim + 3).c_str());

    // true.
    std::getline(s, line);
    delim = line.find("monkey ");
    int true_branch = atoi(line.substr(delim + 7).c_str());

    // false.
    std::getline(s, line);
    delim = line.find("monkey ");
    int false_branch = atoi(line.substr(delim + 7).c_str());

    // empty line.
    std::getline(s, line);

    return Monkey(std::move(items), std::move(op), test, true_branch, false_branch);
  }

  Monkey(std::vector<int> items, std::unique_ptr<Op> op, int test, int true_branch, int false_branch) :
    items_(std::move(items)), op_(std::move(op)), test_(test), true_branch_(true_branch), false_branch_(false_branch) {}

  bool HasItems() const { return !items_.empty(); }

  void AddItem(int item) {
    items_.push_back(item);
  }

  InspectResult Inspect() {
    int value = (*op_)(items_[0]);
    value /= 3;
    int destination = (value % test_ == 0) ? true_branch_ : false_branch_;
    inspect_count_++;
    items_.erase(items_.begin());
    return {
      .value = value,
      .monkey_index = destination,
    };
  }

  int inspect_count() const { return inspect_count_; }

  friend std::ostream& operator <<(std::ostream& s, const Monkey& instance) {
    s << "[";
    for (int i = 0; i < instance.items_.size(); i++) {
      s << instance.items_[i];
      if (i != instance.items_.size() - 1) {
        s << ",";
      }
    }
    s << "]";
    return s;
  }

  private:
  std::vector<int> items_;
  std::unique_ptr<Op> op_;
  int test_;
  int true_branch_;
  int false_branch_;
  int inspect_count_ = 0;
};

void Part1(std::vector<Monkey>& monkies) {
  for (int i = 0; i < 20; i++) {
    for (auto& monkey : monkies) {
      while (monkey.HasItems()) {
        Monkey::InspectResult result = monkey.Inspect();
        monkies[result.monkey_index].AddItem(result.value);
      }
    }
  }
  std::sort(monkies.begin(), monkies.end(), [](const Monkey& m1, const Monkey& m2) -> bool {
    return m1.inspect_count() > m2.inspect_count();
  });

  std::cout << monkies[0].inspect_count() * monkies[1].inspect_count() << std::endl;
}
}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);
  std::vector<aoc2022::Monkey> monkies;
  while (input_file) {
    std::string line;
    std::getline(input_file, line);
    if (line.find("Monkey") != std::string::npos) {
      monkies.emplace_back(aoc2022::Monkey::CreateMonkeyFromStream(input_file));
    }
  }
  aoc2022::Part1(monkies);
  return 0;
}