#pragma once

#include "executor.hpp"

class GameEngine {
private:
  GameState state;
  Program program;
  InstructionExecutor executor;
  int steps;

public:
  const LevelData &level_data;

  GameEngine(const LevelData &level);
  void loadProgram(const Program &new_program);
  const GameState &getState() const;
  bool executeNextInstruction();  // Returns false when program ends
  bool validateOutput() const;
  int getSteps() const;
  int getProgramLength() const;
};
