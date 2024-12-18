#include "d17.h"

#include <iostream>
#include <regex>

#include "parse.h"

namespace {
using namespace aoc;

const std::regex kRegisterRegex("Register ([A|B|C|]): ([0-9]+)");

struct Computer {
  int64_t r_a;
  int64_t r_b;
  int64_t r_c;
  int ip;
  std::vector<int> program;
  std::vector<int> output;
};

Computer ParseComputer(std::ifstream& input) {
  std::string line;
  std::smatch match;
  int64_t a = 0, b = 0, c = 0;
  for (int64_t i = 0; i < 3; ++i) {
    std::getline(input, line);
    std::regex_search(line, match, kRegisterRegex);
    if (match[1] == "A") {
      a = atoi(match[2].str().c_str());
    } else if (match[1] == "B") {
      b = atoi(match[2].str().c_str());
    } else {
      c = atoi(match[2].str().c_str());
    }
  }
  std::getline(input, line);  // newline
  std::getline(input, line);
  std::vector<int> program = ParseInts<int>(line, ',');
  return {
    a,
    b,
    c,
    /*ip=*/0,
    std::move(program),
    {},
  };
}

int64_t DerefCombo(Computer &comp) {
  int64_t combo = comp.program[comp.ip + 1];
  if (combo < 4) {
    return combo;
  } else if (combo == 4) {
    return comp.r_a;
  } else if (combo == 5) {
    return comp.r_b;
  } else if (combo == 6) {
    return comp.r_c;
  }
  std::cout << "combo:" << combo << " ip" << comp.ip << std::endl;
  throw std::runtime_error("Invalid combo");
}

void AssertComboLiteral(Computer& comp) {
  if (comp.program[comp.ip + 1] > 3) {
    std::cout << "Combo " << comp.program[comp.ip + 1] << std::endl;
    throw std::runtime_error("bxl not given a literal at");
  }
}

void adv(Computer& comp) {
  comp.r_a = comp.r_a / pow(2, DerefCombo(comp));
  comp.ip += 2;
}

void bxl(Computer& comp) {
  // AssertComboLiteral(comp);
  comp.r_b = comp.r_b ^ comp.program[comp.ip + 1]; 
  comp.ip += 2;
} 

void bst(Computer& comp) {
  comp.r_b = DerefCombo(comp) % 8;
  comp.ip += 2;
}

void jnz(Computer& comp) {
  if (comp.r_a != 0) {
    AssertComboLiteral(comp);
    comp.ip = comp.program[comp.ip + 1]; 
  } else {
    comp.ip += 2;
  }
}

void bxc(Computer& comp) {
  comp.r_b = comp.r_b ^ comp.r_c;
  comp.ip += 2;
}

void out(Computer& comp) {
  comp.output.push_back(DerefCombo(comp) % 8);
  comp.ip += 2;
}

void bdv(Computer& comp) {
  comp.r_b = comp.r_a / pow(2, DerefCombo(comp));
  comp.ip += 2;
}

void cdv(Computer& comp) {
  comp.r_c = comp.r_a / pow(2, DerefCombo(comp));
  comp.ip += 2;
}

bool Execute(Computer& comp) {
  int64_t op = comp.program[comp.ip];
  if (op == 0) {
    adv(comp);
  } else if (op == 1) {
    bxl(comp);
  } else if (op == 2) {
    bst(comp);
  } else if (op == 3) {
    jnz(comp);
  } else if (op == 4) {
    bxc(comp);
  } else if (op == 5) {
    out(comp);    
  } else if (op == 6) {
    bdv(comp);
  } else if (op == 7) {
    cdv(comp);
  } else {
    std::cout << "Op: " << op << " ip: " << comp.ip << std::endl;
    throw std::runtime_error("Unknown op code.");
  }
  return comp.ip < comp.program.size();
}

std::string DecompileCombo(Computer& comp, int index) {
  int combo = comp.program[index];
  if (combo < 4) {
    return std::to_string(combo); 
  } else if (combo == 4) {
    return "A";
  } else if (combo == 5) {
    return "B";
  } else if (combo == 6) {
    return "C";
  }
  throw std::runtime_error("Unidentified combo");
}

void Decompile(Computer& comp) {
  for (int i = 0; i < comp.program.size(); i += 2) {
    const int& op = comp.program[i];
    if (op == 0) {
      std::cout << "A = A / 2^" << DecompileCombo(comp, i + 1) << std::endl; 
    } else if (op == 1) {
      std::cout << "B = B XOR " << comp.program[i + 1] << std::endl;
    } else if (op == 2) {
      std::cout << "B = " << DecompileCombo(comp, i + 1) << " % 8" << std::endl;
    } else if (op == 3) {
      std::cout << "jmp A" << std::endl;
    } else if (op == 4) {
      std::cout << "B = A XOR C" << std::endl;
    } else if (op == 5) {
      std::cout << "out " << DecompileCombo(comp, i + 1) << " % 8" << std::endl;;
    } else if (op == 6) {
      std::cout << "B = A / 2^" << DecompileCombo(comp, i + 1) << std::endl;
    } else if (op == 7) {
      std::cout << "C = A / 2^" << DecompileCombo(comp, i + 1) << std::endl; 
    }
  }
}

}  // namespace

namespace aoc {
int64_t D17P1(std::ifstream& input) {
  Computer comp = ParseComputer(input);
  while (Execute(comp)) {}
  for (auto itr = comp.output.begin(); itr != comp.output.end(); ++itr) {
    std::cout << *itr;
    if (itr != comp.output.end() - 1) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
  return 0;
}

int64_t D17P2(std::ifstream& input) {
  Computer comp = ParseComputer(input);
  Decompile(comp);
  return 0;
}
}  // namespace aoc
