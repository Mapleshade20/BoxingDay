#pragma once

#include "../core/engine.hpp"

class GameUI {
private:
  GameEngine *engine; // Make GameEngine a pointer member
  int delay_ms;

  // void displayState(const GameState &state);
  void displayError(ExecutionError error, int err_pos);
  void displayExecutionResult(bool success);
  Program readProgramFromUser();

public:
  GameUI() : engine(nullptr), delay_ms(0) {}
  ~GameUI() { delete engine; }

  void setDelay(int ms);
  int menu(); // choose level -> return level number
  void run();
};
