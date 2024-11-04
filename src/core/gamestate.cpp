#include "gamestate.hpp"

GameState::GameState(const LevelData &level) {
  reg = {0, 0, true};
  inbox = level.initial_inbox;
  outbox_buffer.clear();
  inbox_cursor = 0;
  cursor = 0;
  available_tiles = level.available_tiles;
  tiles.clear();
  tiles.resize(available_tiles + 1);
}
