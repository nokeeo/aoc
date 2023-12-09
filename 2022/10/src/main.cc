#include <iostream>
#include <functional>
#include <fstream>
#include <vector>

const std::string INPUT_FILE_NAME = "../input.txt";

namespace aoc2022 {

class Op {
  public:
  virtual ~Op() = default;
  virtual bool Tick(int& r) = 0;
};

class Noop: public Op {
  public:
  bool Tick(int& r) override {
    return true;
  } 
};

class AddOp: public Op {
  public:
  explicit AddOp(int value): ticks_(2), value_(value) {}

  bool Tick(int& r) override {
    ticks_--;
    if (ticks_ == 0) {
      r += value_;
      return true;
    }
    return false;
  }

  private:
  int ticks_;
  int value_;
};

class Machine {
  public:
  Machine(): r_(1) {}

  bool IsSleeping() const { return op_ == nullptr; }
  void AddOp(std::unique_ptr<Op> op) { op_ = std::move(op); }

  void Tick() {
    if (IsSleeping()) { return; }
    if (op_->Tick(r_)) {
      op_.reset();
    }
  }

  void Tick(std::function<std::unique_ptr<Op>()> op_provider, int ticks) {
    while (ticks > 0) {
      if (IsSleeping()) {
        AddOp(std::move(op_provider()));
      }
      Tick();
      ticks--;
    }
  }

  int r() const { return r_; }

  private:
  std::unique_ptr<Op> op_;
  int r_;
};

std::unique_ptr<Op> ParseLine(const std::string& line) {
  if (line == "noop") {
    return std::make_unique<Noop>();
  } else if (line.find("addx") != std::string::npos) {
    return std::make_unique<AddOp>(atoi(line.substr(5).c_str()));
  }
  std::cout << "ERROR: could not parse opp: " << line;
  return nullptr;
}

int Part1(std::function<std::unique_ptr<Op>()> op_provider) {
  Machine m;
  m.Tick(op_provider, 19);
  int strength = m.r() * 20; 
  constexpr int increment = 40;
  for (int i = 59; i < 220; i += increment) {
    m.Tick(op_provider, increment);
    strength += m.r() * (i + 1);
  }
  return strength; 
}

void Part2(std::function<std::unique_ptr<Op>()> op_provider) {
  Machine m;
  for (int y = 0; y < 6; y++) {
    for (int x = 0; x < 40; x++) {
      if (x <= m.r() + 1 && x >= m.r() - 1) {
        std::cout << "#";
      } else {
        std::cout << ".";
      }
      m.Tick(op_provider, 1);
    }
    std::cout << std::endl;
  }
}

}  // namespace aoc2022

int main() {
  std::ifstream input_file;
  input_file.open(INPUT_FILE_NAME);

  std::string line;
  aoc2022::Part2([&input_file]() -> std::unique_ptr<aoc2022::Op> {
    std::string line;
    std::getline(input_file, line);
    return aoc2022::ParseLine(line);
  }); 
  return 0;
}