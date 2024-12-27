#include "ui.hpp"

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>

#include "../fileIO/playerdata.hpp"
#include "instances.hpp"
#include "libtui.hpp"
#include "picker.hpp"
#include "renderer.hpp"

void GameUI::setDelay(int ms) { this->delay_ms = ms; }

void GameUI::checkTerminalSize() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  if (w.ws_col < 100 || w.ws_row < 37) {
    std::cout << "Terminal size is too small. Please resize to at least 100x37."
              << std::endl;
    exit(1);
  }
}

void GameUI::checkAndDownloadLevels() {
  std::string levels_dir =
      std::string(getenv("HOME")) + "/Documents/BoxingDay/levels";
  struct stat info;

  if (stat(levels_dir.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
    std::cout
        << "Levels directory does not exist. Downloading from repository..."
        << std::endl;
    std::string command =
        "git clone https://github.com/tnpschy24/BoxingDayData.git " +
        levels_dir;
    if (system(command.c_str()) != 0) {
      std::cerr << "Failed to clone the repository. Please check your internet "
                   "connection and try again."
                << std::endl;
      exit(1);
    }
  }
}

int GameUI::menu() {
  int level_num = 0;
  DataManager data_manager;
  auto levels = data_manager.readLevelData();
  std::cout << "Unlocked levels: " << std::endl;
  for (auto l : levels) {
    std::cout << "Level " << l.level << " (record: " << l.min_instructions
              << " instructions, " << l.min_steps << " steps)" << std::endl;
  }
  std::cout << "Level " << levels.size() + 1 << std::endl;

  while (true) {
    std::cout << "\nEnter level number or 0 to exit: ";
    std::cin >> level_num;

    if (std::cin.fail() || level_num > levels.size() + 1) {
      std::cin.clear();  // clear the error flag
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n');  // discard invalid input
      std::cout << "Invalid input. Please enter a number." << std::endl;
    } else {
      break;  // valid input
    }
  }

  return level_num;
}

bool GameUI::runtimeInteract() {
  if (kbhit()) {
    char input = getchar();
    switch (input) {
      case 'Q':
        return true;
      default:
        break;
    }
  }
  return false;
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
        char wasted;
        std::cin >> wasted;
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
  checkTerminalSize();
  checkAndDownloadLevels();
  setDelay(1000);
  bool retry = false;
  int level;
  std::cout << "Welcome to Boxing Day!" << std::endl;
  std::cout << "In this game, you will be asked to write a program to sort "
               "inbox numbers into outbox following a given pattern.\n"
            << std::endl;
  while (true) {
    if (!retry) {
      level = menu();
      if (level < 1) {  // Exit signal
        exit(0);
      }
    }
    retry = false;

    // Create new engine for each level
    LevelData level_data;
    try {
      level_data = LevelData::loadLevel(level);
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      continue;
    }
    this->engine = new GameEngine(level_data);

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
    usleep(1000000);

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
        bool quit = runtimeInteract();
        if (quit) break;
        bool continues = engine->executeNextInstruction();

        const GameState state = engine->getState();
        picker.refresh(state, engine->getSteps());
        reg_renderer.refresh(state);
        tiles_renderer.refresh(state);
        sequence_renderer.refresh(state);

        usleep(this->delay_ms * 1000);

        if (!continues) {
          usleep(this->delay_ms * 1000);
          bool success = engine->validateOutput();
          if (success) {
            DataManager data_manager;
            data_manager.writeData(level, engine->getProgramLength(),
                                   engine->getSteps());
          }
          retry = displayExecutionResult(success, ExecutionError::NONE, 0);
          break;
        }
      } catch (ExecutionError error) {
        usleep(this->delay_ms * 1000);
        retry = displayExecutionResult(false, error, engine->getState().cursor);
        break;
      }
    }

    delete this->engine;
    showCursor();
    resetTerminal();
    clearConsole();
  }
}
