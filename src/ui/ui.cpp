#include "ui.hpp"

#include <unistd.h>

#include <iostream>

#include "instances.hpp"
#include "libtui.hpp"
#include "picker.hpp"
#include "renderer.hpp"

void GameUI::setDelay(int ms) { delay_ms = ms; }

int GameUI::menu() {
  // NOTE: Need preview & select level UI
  int level = 1;
  std::cout << "Enter level number (1-3) or 0 to exit: ";
  std::cin >> level;

  return level;
}

// Return true if player retrys, false if player exits
bool GameUI::displayExecutionResult(bool success, ExecutionError error,
                                    int err_pos) {
  Renderer renderer(17, 10);
  renderer.setShape({"***********************************",
                     "*                                 *",
                     "*                                 *",
                     "*                                 *",
                     "*                                 *",
                     "***********************************"});
  renderer.renderShape(0, 0);
  if (success) {
    renderer.renderWord(9, 2, "Level complete!", YELLOW);
    renderer.renderWord(6, 4, "Press any key to return");
    while (true) {
      usleep(1000000 / 60);
      if (kbhit()) {
        return false;
      }
    }
  } else {
    std::string error_message;
    switch (error) {
      case ExecutionError::NONE:
        error_message = "Failed, but don't give up~";
        break;
      default:
        error_message = "Error on instruction " + std::to_string(err_pos + 1);
        break;
    }
    renderer.renderWord(4, 2, error_message, YELLOW);
    renderer.renderWord(2, 4, "'r' to retry; any key to return");
    renderer.renderWord(2, 3, "'h' to see hint");
    while (true) {
      usleep(1000000 / 60);
      if (kbhit()) {
        char input;
        std::cin >> input;
        switch (input) {
          case 'r':
            return true;
          case 'h': {
            std::string hint = "Desired: ";
            for (int n : engine->level_data.expected_outbox) {
              hint += std::to_string(n) + " ";
            }
            renderer.clearArea(2, 3, 20, 1);
            renderer.renderWord(2, 3, hint);
            break;
          }
          default:
            return false;
        }
      }
    }
  }
}

void GameUI::run() {
  setDelay(1000);
  bool retry = false;
  int level = 1;
  while (true) {
    if (!retry) {
      level = menu();
      if (level < 1) {  // Exit signal
        break;
      }
    } else {
      retry = false;
    }

    // Create new engine for each level
    delete engine;
    const LevelData level_data = LevelData::loadLevel(level);
    engine = new GameEngine(level_data);

    // Set up ui components
    setNonBlockingInput();
    hideCursor();
    usleep(500000);
    CanvasRenderer canvas_renderer(level_data);
    RegRenderer reg_renderer(11, 7, engine->getState(), 1.0);
    TilesRenderer tiles_renderer(19, 13, engine->getState());
    SequenceRenderer sequence_renderer(3, 8);
    canvas_renderer.setup();  // have a `clearConsole()` inside
    reg_renderer.setup(engine->getState());
    tiles_renderer.setup(engine->getState());
    sequence_renderer.setup(engine->getState());
    usleep(1500000);

    // Start phase 1: picker
    PickerInteract picker(79, 7, engine->level_data);
    try {
      engine->loadProgram(picker.interact());
    } catch (std::exception &e) {
      showCursor();
      resetTerminal();
      clearConsole();
      continue;
    }

    // Start phase 2: execution
    while (true) {
      try {
        bool continues = engine->executeNextInstruction();

        const GameState state = engine->getState();
        picker.refresh(state, engine->getSteps());
        reg_renderer.refresh(state);
        tiles_renderer.refresh(state);
        sequence_renderer.refresh(state);

        usleep(delay_ms * 1000);

        if (!continues) {
          usleep(delay_ms * 1000);
          retry = displayExecutionResult(engine->validateOutput(),
                                         ExecutionError::NONE, 0);
          showCursor();
          resetTerminal();
          clearConsole();
          break;
        }
      } catch (ExecutionError error) {
        usleep(delay_ms * 1000);
        retry = displayExecutionResult(false, error, engine->getState().cursor);
        showCursor();
        resetTerminal();
        clearConsole();
        break;
      }
    }
  }
}
