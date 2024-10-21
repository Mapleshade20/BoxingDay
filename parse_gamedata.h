#include "include/toml.hpp"
#include <utility>

struct GameData {
  int level;
  std::string title;
  std::string desc;
  int outbox[256];
  int inbox[256];
  std::string available_instructions[8];
  int available_tiles;
};

std::pair<GameData, bool> ParseGameData(std::string path);
