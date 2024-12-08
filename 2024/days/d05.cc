#include "d05.h"

#include <unordered_map>
#include <regex>

#include "parse.h"

namespace {

constexpr int kNotFound = -1;
const std::regex kRuleMatch("([0-9]+)\\|([0-9]+)");

class RuleSet {
  public:
  void Add(std::string l, std::string g) {
    rules_[g].push_back({l, g});
  }

  bool Evaluate(std::string l, std::string g) const {
    auto rules = rules_.find(l);
    if (rules != rules_.end()) {
      for (const auto& rule : rules->second) {
        if (rule.l == g) {
          return false;
        }
      }
    }

    rules = rules_.find(g);
    if (rules != rules_.end()) {
      for (const auto& rule : rules->second) {
        if (rule.l == l) {
          return true;
        }
      }
    }
    return true;
  }
  private:
  struct Rule {
    std::string l;
    std::string g;
  };
  std::unordered_map<std::string, std::vector<Rule>> rules_;
};

RuleSet BuildRuleSet(std::ifstream& input) {
  std::string line;
  RuleSet rule_set;
  std::smatch match;
  while (std::getline(input, line)) {
    if (line == "") {
      return rule_set;
    }
    if (std::regex_search(line, match, kRuleMatch)) {
      rule_set.Add(match[1], match[2]);
    } else {
      throw std::runtime_error("Cannot match regex");
    }
  }
  return rule_set;
}

int IndexOfBrokenRule(const std::vector<std::string>& pages, const RuleSet& rules) {
  for (int i = 1; i < pages.size(); ++i) {
    if (!rules.Evaluate(pages[i - 1], pages[i])) {
      return i - 1;
    }
  }
  return kNotFound;
}
}  // namespace

namespace aoc {
int D5P1(std::ifstream& input) {
  RuleSet rules = BuildRuleSet(input);
  std::string line;
  int total = 0;
  while (std::getline(input, line)) {
    std::vector<std::string> pages = Split(line, ',');
    if (IndexOfBrokenRule(pages, rules) == kNotFound) {
      total += atoi(pages[floor(pages.size() / 2)].c_str());
    }
  }
  return total;
}

int D5P2(std::ifstream& input) {
  RuleSet rules = BuildRuleSet(input);
  std::string line;
  int total = 0;
  while (std::getline(input, line)) {
    std::vector<std::string> pages = Split(line, ',');
    int index = IndexOfBrokenRule(pages, rules);
    if (index != kNotFound) {
      while (index != kNotFound) {
        std::string temp = pages[index];
        pages[index] = pages[index + 1];
        pages[index + 1] = temp;
        index = IndexOfBrokenRule(pages, rules);
      }
      total += atoi(pages[floor(pages.size() / 2)].c_str());
    }
  }
  return total;
}
}  // namespace aoc
