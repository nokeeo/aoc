#include <fstream>
#include <iostream>

#include "grid.h"
#include "parse.h"

/** Day 14: Parabolic Reflector Dish */ 

namespace aoc {
  const std::string kInputFileName = "p1.txt";

  Grid<char> CreateGrid(std::ifstream& input) {
    std::pair<int, int> size = GetGridSize(input);
    Grid<char> grid(size.first, size.second);
    int y = 0;
    std::string line;
    while (std::getline(input, line)) {
      for (int x = 0; x < line.size(); ++x) {
        grid.assign({x, y}, line.at(x));
      }
      ++y;
    }
    return grid;
  }

  Grid<char> TiltBoardNorth(Grid<char>& board) {
    Grid<char> newBoard(board.width(), board.height());
    for (auto itr = board.begin(); itr != board.end(); ++itr) {
      if (*itr != 'O') {
        newBoard.assign(itr.point(), *itr);
        continue;
      }
      Point stop_point = itr.point();
      for (int y = stop_point.y - 1; y >= 0; --y) {
        if (*newBoard.at({stop_point.x, y}) == '.') {
          stop_point = {stop_point.x, y};
        } else {
          break;
        }
      } 

      newBoard.assign(itr.point(), '.');
      newBoard.assign(stop_point, 'O');
    }
    return newBoard;
  }

  Grid<char> TiltBoardSouth(Grid<char>& board) {
    Grid<char> newBoard(board.width(), board.height());
    for (auto itr = board.rbegin(); itr != board.rend(); --itr) {
      if (*itr != 'O') {
        newBoard.assign(itr.point(), *itr);
        continue;
      }
      Point stop_point = itr.point();
      for (int y = stop_point.y + 1; y < board.height(); ++y) {
        if (*newBoard.at({stop_point.x, y}) == '.') {
          stop_point = {stop_point.x, y};
        } else {
          break;
        }
      } 

      newBoard.assign(itr.point(), '.');
      newBoard.assign(stop_point, 'O');
    }
    return newBoard;
  }

  Grid<char> TiltBoardEast(Grid<char>& board) {
    Grid<char> newBoard(board.width(), board.height());
    for (int x = board.width() - 1; x >= 0; --x) {
      for (int y = 0; y < board.height(); ++y) {
        char *c = board.at({x, y});
        if (*c != 'O') {
          newBoard.assign({x, y}, *c);
          continue;
        }
        Point stop_point = {x, y}; 
        for (int x0 = stop_point.x + 1; x < board.width(); ++x0) {
          if (*newBoard.at({x0, stop_point.y}) == '.') {
            stop_point = {x0, stop_point.y};
          } else {
            break;
          }
        } 
        newBoard.assign({x, y}, '.');
        newBoard.assign(stop_point, 'O');
      }
    }
    return newBoard;
  }

  Grid<char> TiltBoardWest(Grid<char>& board) {
    Grid<char> newBoard(board.width(), board.height());
    for (int x = 0; x < board.width(); ++x) {
      for (int y = 0; y < board.height(); ++y) {
        char *c = board.at({x, y});
        if (*c != 'O') {
          newBoard.assign({x, y}, *c);
          continue;
        }
        Point stop_point = {x, y}; 
        for (int x0 = stop_point.x - 1; x0 >= 0; --x0) {
          if (*newBoard.at({x0, stop_point.y}) == '.') {
            stop_point = {x0, stop_point.y};
          } else {
            break;
          }
        } 
        newBoard.assign({x, y}, '.');
        newBoard.assign(stop_point, 'O');
      }
    }
    return newBoard;
  }

  int ComputeLoad(Grid<char>& board) {
    int total = 0;
    for (auto itr = board.begin(); itr != board.end(); ++itr) {
      if (*itr == 'O') {
        total += board.height() - itr.point().y;
      }
    }
    return total;
  }

  int Part1(std::ifstream& input) {
    Grid<char> board = CreateGrid(input);
    board = TiltBoardNorth(board);
    return ComputeLoad(board);
  }

  int Part2(std::ifstream& input) {
    Grid<char> board = CreateGrid(input);
    std::vector<int> loads;
    // We really want to detect a loop in the shift pattern. Too lazy to do so.
    // Coincidentally the load at 1000 runs in the same for 10^9 runs. 
    for (int i = 0; i < 1000; ++i) { 
      board = TiltBoardNorth(board);
      board = TiltBoardWest(board);
      board = TiltBoardSouth(board);
      board = TiltBoardEast(board);
    }
    return ComputeLoad(board);
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