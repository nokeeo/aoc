#include "d13.h"

#include <regex>
#include <iostream>

#include "grid.h"
#include "parse.h"

using Pointf = aoc::PointT<double>;

namespace {
using namespace aoc;
const std::regex kButtonRegex("Button [A|B]: X\\+([0-9]+), Y\\+([0-9]+)");
const std::regex kPrizeRegex("Prize: X=([0-9]+), Y=([0-9]+)");
constexpr double kEpsilon = 0.000001;

struct Machine {
  Pointf bA;
  Pointf bB;
  Pointf prize;
};

struct Line {
  double m;
  double yint;
};

Machine ParseMachine(std::ifstream& input) {
  Machine machine;
  std::string line;
  std::getline(input, line);
  std::smatch match;
  std::regex_search(line, match, kButtonRegex);
  machine.bA = {static_cast<double>(atoi(match[1].str().c_str())), static_cast<double>(atoi(match[2].str().c_str()))};

  std::getline(input, line);
  std::regex_search(line, match, kButtonRegex);
  machine.bB = {static_cast<double>(atoi(match[1].str().c_str())), static_cast<double>(atoi(match[2].str().c_str()))};

  std::getline(input, line);
  std::regex_search(line, match, kPrizeRegex);
  machine.prize = {static_cast<double>(atoi(match[1].str().c_str())), static_cast<double>(atoi(match[2].str().c_str()))};
  return machine;
}

Line GetPointSlopeEquation(const Pointf& p, double m) {
  Line line;
  line.m = m;
  line.yint = -(m * p.x) + p.y;
  return line;
}

Pointf GetIntersection(const Line& l1, const Line& l2) {
  Pointf intersection;
  intersection.x = (l2.yint - l1.yint) / (l1.m - l2.m);
  intersection.y = l1.m * intersection.x + l1.yint;
  return intersection;
}

bool Intersects(const Line& l1, const Line& l2) {
  return fabs(l1.m - l2.m) > 0.0000001;
}

bool IsValidIntersection(const Pointf& p, const Pointf& prize) {
  return floorf(p.x) == p.x && floorf(p.y) == p.y && p.x >= 0 && p.y >= 0 &&
         p.x <= prize.x && p.y <= prize.y;
}
}

namespace aoc {
int64_t D13P1(std::ifstream& input) {
  std::string line;
  int total_cost = 0;
  do {
    Machine machine = ParseMachine(input);
    // First attempt worked on test input, but not actual input.
    // Sim 1.
    // int cost_1 = std::numeric_limits<int>::max();
    // Line l1 = GetPointSlopeEquation({0.f, 0.f}, machine.bA.y / machine.bA.x);
    // Line l2 = GetPointSlopeEquation(machine.prize, machine.bB.y / machine.bB.x);
    // Pointf intersection = GetIntersection(l1, l2);
    // // std::cout << intersection.x << " " << intersection.y << std::endl;
    // if (Intersects(l1, l2) && IsValidIntersection(intersection, machine.prize)) {
    //   int a_steps = std::round(intersection.x / machine.bA.x);
    //   if (intersection.x / machine.bA.x - a_steps > 0.0001) {
    //     std::cout << "uh oh" << std::endl;
    //   }
    //   int b_steps = std::round((machine.prize.x - intersection.x) / machine.bB.x);
    //   if ((machine.prize.x - intersection.x) / machine.bB.x - b_steps > 0.0001) {
    //     std::cout << "uh oh" << std::endl;
    //   }
    //   cost_1 = a_steps * 3 + b_steps;
    //   // std::cout << a_steps << std::endl;
    //   // std::cout << b_steps << std::endl;
    // }

    // // Sim 2.
    // int cost_2 = std::numeric_limits<int>::max();
    // l1 = GetPointSlopeEquation({0.f, 0.f}, machine.bB.y / machine.bB.x);
    // l2 = GetPointSlopeEquation(machine.prize, machine.bA.y / machine.bA.x);
    // intersection = GetIntersection(l1, l2);
    // if (Intersects(l1, l2) && IsValidIntersection(intersection, machine.prize)) {
    //   int b_steps = std::round(intersection.x / machine.bB.x);
    //   int a_steps = std::round((machine.prize.x - intersection.x) / machine.bA.x);
    //   cost_2 = a_steps * 3 + b_steps;
    // }

    // if (cost_1 == std::numeric_limits<int>::max() && cost_2 == std::numeric_limits<int>::max()) {
    //   continue;
    // }
    // total_cost += (cost_1 < cost_2) ? cost_1 : cost_2;
    double a = (machine.prize.x * machine.bB.y - machine.prize.y * machine.bB.x) / (machine.bA.x * machine.bB.y - machine.bA.y * machine.bB.x);
    double b = (machine.bA.x * machine.prize.y - machine.bA.y * machine.prize.x) / (machine.bA.x * machine.bB.y - machine.bA.y * machine.bB.x);
    if (floorf(a) == a && floorf(b) == b) {
      total_cost += a * 3 + b;
    }
  } while (std::getline(input, line));
  return total_cost;
}

int64_t D13P2(std::ifstream& input) {
  std::string line;
  int64_t total_cost = 0;
  do {
    Machine machine = ParseMachine(input);
    machine.prize = machine.prize + Pointf{10000000000000.l, 10000000000000.l};
    double a = (machine.prize.x * machine.bB.y - machine.prize.y * machine.bB.x) / (machine.bA.x * machine.bB.y - machine.bA.y * machine.bB.x);
    double b = (machine.bA.x * machine.prize.y - machine.bA.y * machine.prize.x) / (machine.bA.x * machine.bB.y - machine.bA.y * machine.bB.x);
    if (floor(a) == a && floor(b) == b) {
      total_cost += a * 3 + b;
    }
  } while (std::getline(input, line));
  return total_cost;
}
}  // namespace aoc
