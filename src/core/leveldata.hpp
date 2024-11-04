#pragma once

#include "types.hpp"

class LevelData {
public:
  int level;
  std::string title;
  std::string description;
  std::vector<int> expected_outbox;
  std::vector<int> initial_inbox;
  std::vector<InstructionType> available_instructions;
  int available_tiles;

  static LevelData loadLevel(int level_number);
  static LevelData previewLevel(int level_number);
};
