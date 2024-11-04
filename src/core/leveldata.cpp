#include "leveldata.hpp"

LevelData LevelData::loadLevel(int level_number) {
  LevelData level_data;
  switch (level_number) {
  case 1:
    level_data.initial_inbox = {1, 2};
    level_data.expected_outbox = {1, 2};
    level_data.available_tiles = 0;
    level_data.available_instructions = {InstructionType::INBOX,
                                         InstructionType::OUTBOX};
    break;
  case 2:
    level_data.initial_inbox = {3, 9, 5, 1, -2, -2, 9, -9};
    level_data.expected_outbox = {-6, 6, 4, -4, 0, 0, 18, -18};
    level_data.available_tiles = 3;
    level_data.available_instructions = {
        InstructionType::INBOX,    InstructionType::OUTBOX,
        InstructionType::ADD,      InstructionType::SUB,
        InstructionType::JUMP,     InstructionType::COPYTO,
        InstructionType::COPYFROM, InstructionType::JUMPIFZERO};
    break;
  case 3:
    level_data.initial_inbox = {6, 2, 7, 7, -9, 3, -3, -3};
    level_data.expected_outbox = {7, -3};
    level_data.available_tiles = 3;
    level_data.available_instructions = {
        InstructionType::INBOX,    InstructionType::OUTBOX,
        InstructionType::ADD,      InstructionType::SUB,
        InstructionType::JUMP,     InstructionType::COPYTO,
        InstructionType::COPYFROM, InstructionType::JUMPIFZERO};
    break;
  default:
    throw ExecutionError::LEVEL_NOT_FOUND;
  }
  return level_data;
}
