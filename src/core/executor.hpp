#pragma once

#include "gamestate.hpp"

class InstructionExecutor {
private:
  void executeInbox(GameState &state);
  void executeOutbox(GameState &state);
  void executeAdd(GameState &state, int param);
  void executeSub(GameState &state, int param);
  void executeCopyTo(GameState &state, int param);
  void executeCopyFrom(GameState &state, int param);
  void executeJump(GameState &state, int param);
  void executeJumpIfZero(GameState &state, int param);
  bool is_successful_jump;

public:
  int program_size;
  InstructionExecutor();
  void executeInstruction(GameState &state, const Instruction &inst);
};
