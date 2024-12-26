#pragma once

#include "../core/engine.hpp"

class GameUI {
private:
  GameEngine *engine;  // Make GameEngine a pointer member
  int delay_ms;

  // Show the main menu and return the level number
  int menu();

  // Return true if player retrys, false if player exits
  bool displayExecutionResult(bool success, ExecutionError error, int err_pos);

  // Read keyboard input once. Return true if player quits
  bool runtimeInteract();

  void checkTerminalSize();

public:
  GameUI() : engine(nullptr), delay_ms(0) {}
  ~GameUI() { delete engine; }

  void setDelay(int ms);
  void run();
};
