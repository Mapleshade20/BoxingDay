#include "include/toml.hpp"
// #include <iostream>
#include <utility>

struct GameData {
  int level;
  std::string title;
  std::string desc;
  int outbox_size;
  int outbox[256];
  int inbox_size;
  int inbox[256];
  std::string available_instructions[8];
  int available_tiles;
};

std::pair<GameData, bool> ParseGameData(std::string path);
