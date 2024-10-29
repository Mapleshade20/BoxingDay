#include "types.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

/*
 * leveldata.h - Level configuration
 */

class LevelData {
public:
  int level;
  std::string title;
  std::string description;
  std::vector<int> expected_outbox;
  std::vector<int> initial_inbox;
  std::vector<InstructionType> available_instructions;
  int available_tiles;

  static LevelData loadLevel(int level_number);
  static LevelData previewLevel(int level_number);
};

// leveldata.cpp
LevelData LevelData::loadLevel(int level_number) {
  LevelData level_data;
  switch (level_number) {
  case 1:
    level_data.initial_inbox = {1, 2};
    level_data.expected_outbox = {1, 2};
    level_data.available_tiles = 0;
    level_data.available_instructions = {InstructionType::INBOX,
                                         InstructionType::OUTBOX};
    break;
  case 2:
    level_data.initial_inbox = {3, 9, 5, 1, -2, -2, 9, -9};
    level_data.expected_outbox = {-6, 6, 4, -4, 0, 0, 18, -18};
    level_data.available_tiles = 3;
    level_data.available_instructions = {
        InstructionType::INBOX,    InstructionType::OUTBOX,
        InstructionType::ADD,      InstructionType::SUB,
        InstructionType::JUMP,     InstructionType::COPYTO,
        InstructionType::COPYFROM, InstructionType::JUMPIFZERO};
    break;
  case 3:
    level_data.initial_inbox = {6, 2, 7, 7, -9, 3, -3, -3};
    level_data.expected_outbox = {7, -3};
    level_data.available_tiles = 3;
    level_data.available_instructions = {
        InstructionType::INBOX,    InstructionType::OUTBOX,
        InstructionType::ADD,      InstructionType::SUB,
        InstructionType::JUMP,     InstructionType::COPYTO,
        InstructionType::COPYFROM, InstructionType::JUMPIFZERO};
    break;
  default:
    throw ExecutionError::LEVEL_NOT_FOUND;
  }
  return level_data;
}

/*
 * gamestate.h - Game state management
 */

class GameState {
public:
  Register reg;
  std::vector<Tile> tiles;
  std::vector<int> inbox;
  std::vector<int> outbox_buffer;
  size_t inbox_cursor;
  size_t cursor;
  int available_tiles;

  GameState(const LevelData &level);
};

// gamestate.cpp
GameState::GameState(const LevelData &level) {
  reg = {0, 0, 0, true};
  inbox = level.initial_inbox;
  outbox_buffer.clear();
  inbox_cursor = 0;
  cursor = 0;
  available_tiles = level.available_tiles;
  tiles.clear();
  tiles.resize(available_tiles + 1);
}

/*
 * instruction.h - Instruction handling
 */

class Instruction {
public:
  InstructionType type;
  int param;

  static Instruction fromString(const std::string &name, int param = 0);
};
// TODO: 2. instruction.cpp fromString()

/*
 * program.h - Program management
 */

class Program {
private:
  std::vector<Instruction> instructions;

public:
  void addInstruction(const Instruction &inst);
  void clear();
  const Instruction &at(size_t index) const;
  size_t size() const;
};

// program.cpp
void Program::addInstruction(const Instruction &inst) {
  instructions.push_back(inst);
}
void Program::clear() { instructions.clear(); }
const Instruction &Program::at(size_t index) const {
  return instructions.at(index);
}
size_t Program::size() const { return instructions.size(); }

/*
 * executor.h - Instruction execution logic
 */

class InstructionExecutor {
private:
  void executeInbox(GameState &state);
  void executeOutbox(GameState &state);
  void executeAdd(GameState &state, int param);
  void executeSub(GameState &state, int param);
  void executeCopyTo(GameState &state, int param);

public:
  void executeInstruction(GameState &state, const Instruction &inst);
};

// executor.cpp
void InstructionExecutor::executeInstruction(GameState &state,
                                             const Instruction &inst) {
  switch (inst.type) {
  case InstructionType::INBOX:
    executeInbox(state);
    break;
  case InstructionType::OUTBOX:
    executeOutbox(state);
    break;
  // ... add other cases for different instruction types
  default:
    throw ExecutionError::INVALID_INSTRUCTION;
  }
}

void InstructionExecutor::executeInbox(GameState &state) {
  if (state.inbox_cursor >= state.inbox.size()) {
    throw ExecutionError::EMPTY_INBOX;
  }

  state.reg.dest_tile = -1;
  state.reg.current_tile = state.reg.dest_tile;
  state.reg.is_empty = false;
  state.reg.hand = state.inbox[state.inbox_cursor++];
}
// TODO: 3. implement other execute methods

/*
 * engine.h - Core game logic, destroyed after each level ends
 */

class GameEngine {
private:
  GameState state;
  Program program;
  const LevelData &level_data;
  InstructionExecutor executor;

public:
  GameEngine(const LevelData &level);
  void loadProgram(const Program &new_program);
  const GameState &getState() const;
  bool executeNextInstruction(); // Returns false when program ends
  bool validateOutput() const;
};

// engine.cpp
GameEngine::GameEngine(const LevelData &data) : state(data), level_data(data) {}
void GameEngine::loadProgram(const Program &new_program) {
  program = new_program;
}
const GameState &GameEngine::getState() const { return state; }
bool GameEngine::validateOutput() const {
  return state.outbox_buffer == level_data.expected_outbox;
}
bool GameEngine::executeNextInstruction() {
  if (state.cursor >= program.size()) {
    return false;
  }

  const Instruction &current = program.at(state.cursor);

  if (current.type == InstructionType::END ||
      (current.type == InstructionType::INBOX &&
       state.inbox_cursor >= state.inbox.size())) {
    return false;
  }

  executor.executeInstruction(state, current);
  return true;
}

/*
 * UI.h - User interface
 */

class GameUI {
private:
  int delay_ms;
  void displayState(const GameState &state);
  void displayError(ExecutionError error, int err_pos);
  void displayExecutionResult(bool success);
  Program readProgramFromUser();

public:
  GameUI() : delay_ms(0) {} // NOTE: Change to a non-zero value after OJ
  void setDelay(int ms);
  int menu(); // choose level -> return level number
  void run();
};

// UI.cpp
void GameUI::setDelay(int ms) { delay_ms = ms; }

Program GameUI::readProgramFromUser() {}
// TODO: 5. implement readProgramFromUser()

int GameUI::menu() {
  // NOTE: preview & select level UI (for OJ this is just cin a number)
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

void GameUI::run() {
  while (true) {
    int level = menu();
    if (level == -1) { // Exit signal
      break;
    }
    const LevelData level_data = LevelData::loadLevel(level);
    GameEngine engine(level_data);
    Program program = readProgramFromUser();
    engine.loadProgram(program);

    while (true) {
      // Execute one instruction
      try {
        bool continues = engine.executeNextInstruction();

        // Display the current state after each instruction
        displayState(engine.getState());

        // Control speed of execution
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

        if (!continues) {
          break;
        }
      } catch (ExecutionError error) {
        displayError(error, engine.getState().cursor);
        return;
      }
    }

    // Show final result
    bool result = engine.validateOutput();
    displayExecutionResult(result);

    break; // NOTE: Remove this line to play all levels after OJ

    // SAVE FEATURE GOES HERE!
  }
}

/*
 * main.cpp - Program entry point
 */

int main() {
  try {

    GameUI ui;

    // ui.home();
    ui.run();

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

/*

Benefits of this structure:
1. Clear separation of concerns
2. Each class has a single responsibility
3. Easy to test individual components
4. Easy to modify or extend functionality
5. Clear interfaces between components
6. Better error handling
7. Better state management
8. Easy to add new features (save/load, different UIs, etc.)
9. More maintainable and scalable

*/
