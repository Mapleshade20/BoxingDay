#pragma once

#include "leveldata.hpp"

class GameState {
public:
  Register reg;
  std::vector<Tile> tiles;
  std::vector<int> inbox;
  std::vector<int> outbox_buffer;
  int inbox_cursor;
  int cursor;
  int available_tiles;

  GameState(const LevelData &level);
};
