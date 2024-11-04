#pragma once

#include "executor.hpp"

class GameEngine {
private:
  GameState state;
  Program program;
  InstructionExecutor executor;

public:
  GameEngine(const LevelData &level);
  void loadProgram(const Program &new_program);
  const LevelData &level_data;
  const GameState &getState() const;
  bool executeNextInstruction(); // Returns false when program ends
  bool validateOutput() const;
};
