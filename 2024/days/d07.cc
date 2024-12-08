#include "d07.h"

#include <regex>
#include <vector>
#include <iostream>

#include "parse.h"

namespace {
using namespace aoc;

const std::regex kEquationRegex("(^[0-9]+): (.*)");

struct Equation {
  int64_t result;
  std::vector<int64_t> parts;
};

int64_t Combine(int64_t a, int64_t b) {
  std::stringstream ss;
  ss << a << b;
  return strtoll(ss.str().c_str(), NULL, 10);
}

Equation ParseEquation(const std::string& line) {
  std::smatch match;
  if (std::regex_match(line, match, kEquationRegex)) {
    return {
      .result = strtoll(match[1].str().c_str(), NULL, 10),
      .parts = ParseInts<int64_t>(match[2].str()),
    };
  } else {
    throw std::runtime_error("Could not parse equation");
  }
  return {};
}

bool _FindOperators(const Equation& eq, int64_t partial, int i, bool combine = false) {
  if (i == eq.parts.size()) { 
    return partial == eq.result;
  }

  return _FindOperators(eq, partial + eq.parts[i], i + 1, combine) ||
         _FindOperators(eq, partial * eq.parts[i], i + 1, combine) ||
         (combine && _FindOperators(eq, Combine(partial, eq.parts[i]), i + 1, combine));
}

bool FindOperators(const Equation& eq, bool combine = false) {
  if (eq.parts.size() == 0) {
    return eq.result == 0;
  } else if (eq.parts.size() == 1) {
    return eq.result == eq.parts[0];
  }
  return _FindOperators(eq, eq.parts[0], 1, combine);
}
}  // namespace

namespace aoc {

int64_t D7P1(std::ifstream& input) {
  std::string line;
  int64_t count = 0;
  while (std::getline(input, line)) {
    Equation eq = ParseEquation(line);
    if (FindOperators(eq)) {
      count += eq.result;
    }
  }
  return count;
}

int64_t D7P2(std::ifstream& input) {
  std::string line;
  int64_t count = 0;
  while (std::getline(input, line)) {
    Equation eq = ParseEquation(line);
    if (FindOperators(eq, true)) {
      count += eq.result;
    }
  }
  return count;
}
}  // namespace aoc
