#include "ui.hpp"

#include <unistd.h>

#include <iostream>

#include "instances.hpp"
#include "libtui.hpp"
#include "picker.hpp"

void GameUI::setDelay(int ms) { delay_ms = ms; }

Program GameUI::readProgramFromUser() {
  PickerInteract picker(79, 7, engine->level_data);
  return picker.interact();
  // int n_ins;
  // std::cin >> n_ins;
  // std::cin.ignore(1000, '\n');  // Clear newline after reading n_ins
  // Program program;
  //
  // for (int t = 0; t < n_ins; t++) {
  //   std::string s_input, command;
  //   std::getline(std::cin, s_input);
  //   std::stringstream stream(s_input);
  //   int param = -1;
  //
  //   stream >> command;
  //   if (command.empty()) {
  //     command = "error";
  //     continue;
  //   }
  //
  //   if (!stream.eof()) {
  //     std::string param_str;
  //     stream >> param_str;
  //
  //     if (!param_str.empty() && stream.eof()) {
  //       try {
  //         param = std::stoi(param_str);
  //       } catch (const std::invalid_argument &) {
  //         command = "error";
  //       } catch (const std::out_of_range &) {
  //         command = "error";
  //       }
  //     } else {
  //       command = "error";  // More than 2 words or invalid param
  //     }
  //   }
  //
  //   Instruction i = Instruction::fromString(command, param);
  //   if (std::none_of(engine->level_data.available_instructions.begin(),
  //                    engine->level_data.available_instructions.end(),
  //                    [&i](InstructionType type) { return type == i.type; }))
  //                    {
  //     i.type = InstructionType::ERROR;
  //   }
  //   program.addInstruction(i);
  // }
  // return program;
}

int GameUI::menu() {
  // NOTE: Need preview & select level UI
  int level = 1;
  std::cout << "Enter level number (1-3) or 0 to exit: ";
  std::cin >> level;

  return level;
}

void GameUI::displayError(ExecutionError error, int err_pos) {
  std::cout << "Error on instruction " << err_pos + 1 << std::endl;
}

void GameUI::displayExecutionResult(bool success) {
  if (success) {
    std::cout << "Success" << std::endl;
  } else {
    std::cout << "Fail" << std::endl;
  }
}

void GameUI::run() {
  setDelay(1000);
  while (true) {
    int level = menu();
    if (level < 1) {  // Exit signal
      break;
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
    canvas_renderer.setup();
    reg_renderer.setup(engine->getState());
    tiles_renderer.setup(engine->getState());
    sequence_renderer.setup(engine->getState());
    usleep(1500000);

    // Start phase 1: picker
    Program program = readProgramFromUser();
    engine->loadProgram(program);

    // Start phase 2: execution
    while (true) {
      try {
        bool continues = engine->executeNextInstruction();
        reg_renderer.refresh(engine->getState());
        tiles_renderer.refresh(engine->getState());
        sequence_renderer.refresh(engine->getState());

        usleep(delay_ms * 1000);

        if (!continues) {
          usleep(delay_ms * 1000);
          bool result = engine->validateOutput();
          showCursor();
          resetTerminal();
          clearConsole();
          displayExecutionResult(result);
          break;
        }
      } catch (ExecutionError error) {
        usleep(delay_ms * 1000);
        showCursor();
        resetTerminal();
        clearConsole();
        displayError(error, engine->getState().cursor);
        break;
      }
    }
  }
}
