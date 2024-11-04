#include "ui.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <thread>

void GameUI::setDelay(int ms) { delay_ms = ms; }
Program GameUI::readProgramFromUser() {
  int n_ins;
  std::cin >> n_ins;
  std::cin.ignore(1000, '\n'); // Clear newline after reading n_ins
  Program program;

  for (int t = 0; t < n_ins; t++) {
    std::string s_input, command;
    std::getline(std::cin, s_input);
    std::stringstream stream(s_input);
    int param = -1;

    stream >> command;
    if (command.empty()) {
      command = "error";
      continue;
    }

    if (!stream.eof()) {
      std::string param_str;
      stream >> param_str;

      if (!param_str.empty() && stream.eof()) {
        try {
          param = std::stoi(param_str);
        } catch (const std::invalid_argument &) {
          command = "error";
        } catch (const std::out_of_range &) {
          command = "error";
        }
      } else {
        command = "error"; // More than 2 words or invalid param
      }
    }

    Instruction i = Instruction::fromString(command, param);
    if (std::none_of(engine->level_data.available_instructions.begin(),
                     engine->level_data.available_instructions.end(),
                     [&i](InstructionType type) { return type == i.type; })) {
      i.type = InstructionType::ERROR;
    }
    program.addInstruction(i);
  }
  return program;
}

int GameUI::menu() {
  // NOTE: Need preview & select level UI
  int level = 1;
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

// INFO: Only for debugging!
// void GameUI::displayState(const GameState &state) {
//   std::cout << "[[Current State]]" << std::endl;
//   std::cout << "Inbox Cursor: " << state.inbox_cursor << std::endl;
//   std::cout << "Cursor: " << state.cursor << std::endl;
//   std::cout << "Hand: "
//             << (state.reg.is_empty ? "Empty" :
//             std::to_string(state.reg.hand))
//             << std::endl;
//   std::cout << "Outbox Buffer: ";
//   for (const auto &value : state.outbox_buffer) {
//     std::cout << value << " ";
//   }
//   std::cout << std::endl;
// }

void GameUI::run() {
  while (true) {
    int level = menu();
    if (level == -1) { // Exit signal
      break;
    }

    // Create new engine for each level
    delete engine;
    const LevelData level_data = LevelData::loadLevel(level);
    engine = new GameEngine(level_data);

    Program program = readProgramFromUser();
    engine->loadProgram(program);

    while (true) {
      try {
        bool continues = engine->executeNextInstruction();

        // INFO: Only for debugging!
        // displayState(engine->getState());
        // std::cout << "executeNextInstruction() returned: "
        //           << (continues ? "true" : "false") << "\n"
        //           << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

        if (!continues) {
          break;
        }
      } catch (ExecutionError error) {
        displayError(error, engine->getState().cursor);
        return;
      }
    }

    bool result = engine->validateOutput();
    displayExecutionResult(result);

    break; // NOTE: Remove this line to play all levels after OJ
  }
}
