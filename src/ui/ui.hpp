#pragma once

#include "../core/engine.hpp"

class GameUI {
private:
  GameEngine *engine;  // Make GameEngine a pointer member
  int delay_ms;

  bool displayExecutionResult(bool success, ExecutionError error, int err_pos);
  // Program readProgramFromUser();

public:
  GameUI() : engine(nullptr), delay_ms(0) {}
  ~GameUI() { delete engine; }

  void setDelay(int ms);
  int menu();  // choose level -> return level number
  void run();
};
