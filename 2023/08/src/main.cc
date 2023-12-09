#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <regex>

/** Day 8: Haunted Wasteland */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  const std::regex kNodeRegex("([A-Z]{3}) = \\(([A-Z]{3}), ([A-Z]{3})\\)");

  struct Node {
    std::string l; 
    std::string r; 
    std::string v;
  };

  std::unordered_map<std::string, Node> BuildGraph(std::ifstream& input) {
    std::unordered_map<std::string, Node> graph;
    std::string line;
    std::smatch match;
    while (std::getline(input, line)) {
      if (!std::regex_search(line, match, kNodeRegex)) {
        throw std::runtime_error("Failed to parse line.");
      }
      Node n = Node{
        .l = match[2].str(),
        .r = match[3].str(),
        .v = match[1].str(),
      };
      graph[match[1].str()] = std::move(n);
    }
    return graph;
  }

  int TraverseGraph(Node* root, const std::unordered_map<std::string, Node>& graph, const std::string& directions) {
    int total = 0;
    int dir_index = 0;
    const Node *node = root;
    while(node->v != "ZZZ") {
      if (directions.at(dir_index) == 'L') {
        node = &(graph.find(node->l)->second); 
      } else if (directions.at(dir_index == 'R')) {
        node = &(graph.find(node->r)->second);
      } else {
        throw std::runtime_error("Unrecognized character");
      }
      ++total;
      dir_index = (dir_index + 1) % directions.size();
    }
    return total; 
  }

  int Part1(std::ifstream& input) {
    std::string directions;
    std::getline(input, directions);

    std::string throw_away;
    std::getline(input, throw_away);
    std::unordered_map<std::string, Node> graph = BuildGraph(input);
    return TraverseGraph(&(graph["AAA"]), graph, directions);
  }

  bool GhostEndNodes(const std::vector<const Node*>& nodes) {
    for (const auto& n : nodes) {
      if (n->v.at(n->v.size() - 1) != 'Z') {
        return false;
      }
    }
    return true;
  }

  int GhostTraverse(const std::vector<const Node*>& start, const std::unordered_map<std::string, Node>& graph, const std::string& directions) {
    // This was my original solution.  It was not completing in a reasonable time.  Folks online recommended computing the distance from each
    // start node to the first node that ends with Z. The code for computing the length is below. Use with a lcm calculator.
    // std::vector<const Node*> nodes = start;
    // int dir_index = 0;
    // int steps = 0;
    // while (!GhostEndNodes(nodes)) {
    //   std::vector<const Node*> next_nodes;
    //   next_nodes.reserve(nodes.size());
    //   for (const auto& n : nodes) {
    //     if (directions.at(dir_index) == 'L') {
    //       next_nodes.push_back(&(graph.find(n->l)->second));
    //     } else if (directions.at(dir_index) == 'R') {
    //       next_nodes.push_back(&(graph.find(n->r)->second));
    //     } else {
    //       throw std::runtime_error("Failed to recognize character");
    //     }
    //   }
    //   nodes = next_nodes;
    //   dir_index = (dir_index + 1) % directions.size();
    //   ++steps;
    // }
    // return steps;
    int total = 0;
    for (const auto& s : start) {
      int dir_index = 0;
      int steps = 0;
      const Node* node = s; 
      while (node->v.at(node->v.size() - 1) != 'Z') {
        if (directions.at(dir_index) == 'L') {
          node = &(graph.find(node->l)->second);
        } else if (directions.at(dir_index) == 'R') {
          node = &(graph.find(node->r)->second);
        } else {
          throw std::runtime_error("Failed to recognize character");
        }
        dir_index = (dir_index + 1) % directions.size();
        ++steps;
      }
      std::cout << "Take the LCM of the following:"
      std::cout << steps << std::endl;
      if (total == 0) {
        total = steps;
      } else {
        std::lcm(total, steps);
      }
      std::cout << "------" << std::endl;
    }
    return total;
  }

  int Part2(std::ifstream& input) {
    std::string directions;
    std::getline(input, directions);

    std::string throw_away;
    std::getline(input, throw_away);
    std::unordered_map<std::string, Node> graph = BuildGraph(input);

    std::vector<const Node*> start_nodes;
    for (const auto& n : graph) {
      if (n.first.at(n.first.size() - 1) == 'A') {
        start_nodes.push_back(&(n.second));
      }
    }
    return GhostTraverse(start_nodes, graph, directions);
  }
}  // namespace aoc

int main() {
  std::ifstream input_file;
  input_file.open(aoc::kInputFileName);
  std::cout << aoc::Part1(input_file) << std::endl;
  input_file.clear();
  input_file.seekg(0);
  std::cout << aoc::Part2(input_file) << std::endl;
  return 0;
}