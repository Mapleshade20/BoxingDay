#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

enum class InstructionType {
  INBOX,
  OUTBOX,
  ADD,
  SUB,
  COPYTO,
  COPYFROM,
  JUMP,
  JUMPIFZERO,
  END,
  ERROR
};

enum class ExecutionError {
  NONE,
  LEVEL_NOT_FOUND,
  INVALID_INSTRUCTION,
  EMPTY_HAND,
  EMPTY_TILE,
  INVALID_JUMP,
  OUT_OF_BOUNDS,
};

struct Register {
  int current_tile;
  int hand;
  bool is_empty;
};

struct Tile {
  int value;
  bool is_empty;
};

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
  int inbox_cursor;
  int cursor;
  int available_tiles;

  GameState(const LevelData &level);
};

// gamestate.cpp
GameState::GameState(const LevelData &level) {
  reg = {0, 0, true};
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

  static Instruction fromString(const std::string &name, int param);
};

// instruction.cpp
Instruction Instruction::fromString(const std::string &name, int param) {
  Instruction ans;
  if (name == "error") {
    ans.type = InstructionType::ERROR;
    ans.param = -2;
    return ans;
  }
  ans.param = param;
  if (name == "inbox" && param == -1)
    ans.type = InstructionType::INBOX;
  else if (name == "outbox" && param == -1)
    ans.type = InstructionType::OUTBOX;
  else if (name == "add" && param != -1)
    ans.type = InstructionType::ADD;
  else if (name == "sub" && param != -1)
    ans.type = InstructionType::SUB;
  else if (name == "copyto" && param != -1)
    ans.type = InstructionType::COPYTO;
  else if (name == "copyfrom" && param != -1)
    ans.type = InstructionType::COPYFROM;
  else if (name == "jump" && param != -1)
    ans.type = InstructionType::JUMP;
  else if (name == "jumpifzero" && param != -1)
    ans.type = InstructionType::JUMPIFZERO;
  else {
    ans.type = InstructionType::ERROR;
    ans.param = -2;
  }
  return ans;
}

/*
 * program.h - Program management
 */

class Program {
private:
  std::vector<Instruction> instructions;

public:
  void addInstruction(const Instruction &inst);
  void clear();
  const Instruction &at(int index) const;
  const int size() const;
};

// program.cpp
void Program::addInstruction(const Instruction &inst) {
  instructions.push_back(inst);
}
void Program::clear() { instructions.clear(); }
const Instruction &Program::at(int index) const {
  return instructions.at(index);
}
const int Program::size() const { return instructions.size(); }

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
  void executeCopyFrom(GameState &state, int param);
  void executeJump(GameState &state, int param);
  void executeJumpIfZero(GameState &state, int param);
  bool is_successful_jump;

public:
  int program_size;
  InstructionExecutor();
  void executeInstruction(GameState &state, const Instruction &inst);
};

// executor.cpp
InstructionExecutor::InstructionExecutor()
    : is_successful_jump(false), program_size(0) {}
void InstructionExecutor::executeInstruction(GameState &state,
                                             const Instruction &inst) {
  switch (inst.type) {
  case InstructionType::INBOX:
    executeInbox(state);
    break;
  case InstructionType::OUTBOX:
    executeOutbox(state);
    break;
  case InstructionType::ADD:
    executeAdd(state, inst.param);
    break;
  case InstructionType::SUB:
    executeSub(state, inst.param);
    break;
  case InstructionType::COPYTO:
    executeCopyTo(state, inst.param);
    break;
  case InstructionType::COPYFROM:
    executeCopyFrom(state, inst.param);
    break;
  case InstructionType::JUMP:
    executeJump(state, inst.param);
    break;
  case InstructionType::JUMPIFZERO:
    executeJumpIfZero(state, inst.param);
    break;
  default:
    throw ExecutionError::INVALID_INSTRUCTION;
  }

  if (!is_successful_jump) {
    state.cursor++; // NOTE: UI runs when `cursor` already points to next
  }
  is_successful_jump = false;
}

void InstructionExecutor::executeInbox(GameState &state) {
  state.reg.current_tile = -1;
  state.reg.is_empty = false;
  state.reg.hand = state.inbox[state.inbox_cursor];
  state.inbox_cursor++;
}

void InstructionExecutor::executeOutbox(GameState &state) {
  if (state.reg.is_empty == true) {
    throw ExecutionError::EMPTY_HAND;
  }
  state.reg.current_tile = -2;
  state.reg.is_empty = true;
  state.outbox_buffer.push_back(state.reg.hand);
}

void InstructionExecutor::executeAdd(GameState &state, int param) {
  if (state.reg.is_empty == true)
    throw ExecutionError::EMPTY_HAND;
  if (param >= state.available_tiles)
    throw ExecutionError::OUT_OF_BOUNDS;
  if (state.tiles[param].is_empty)
    throw ExecutionError::EMPTY_TILE;

  state.reg.current_tile = param;
  state.reg.hand += state.tiles[param].value;
}

void InstructionExecutor::executeSub(GameState &state, int param) {
  if (state.reg.is_empty == true)
    throw ExecutionError::EMPTY_HAND;
  if (param >= state.available_tiles)
    throw ExecutionError::OUT_OF_BOUNDS;
  if (state.tiles[param].is_empty)
    throw ExecutionError::EMPTY_TILE;

  state.reg.current_tile = param;
  state.reg.hand -= state.tiles[param].value;
}

void InstructionExecutor::executeCopyTo(GameState &state, int param) {
  if (state.reg.is_empty == true)
    throw ExecutionError::EMPTY_HAND;
  if (param >= state.available_tiles)
    throw ExecutionError::OUT_OF_BOUNDS;

  state.reg.current_tile = param;
  state.tiles[param].is_empty = false;
  state.tiles[param].value = state.reg.hand;
}

void InstructionExecutor::executeCopyFrom(GameState &state, int param) {
  if (param >= state.available_tiles)
    throw ExecutionError::OUT_OF_BOUNDS;
  if (state.tiles[param].is_empty)
    throw ExecutionError::EMPTY_TILE;

  state.reg.current_tile = param;
  state.reg.is_empty = false;
  state.reg.hand = state.tiles[param].value;
}

void InstructionExecutor::executeJump(GameState &state, int param) {
  if (param <= 0 || param >= program_size + 1 || param == state.cursor + 1)
    throw ExecutionError::INVALID_JUMP;

  state.cursor = param - 1;
  is_successful_jump = true;
}

void InstructionExecutor::executeJumpIfZero(GameState &state, int param) {
  if (param <= 0 || param >= program_size + 1 || param == state.cursor + 1)
    throw ExecutionError::INVALID_JUMP;
  if (state.reg.is_empty)
    throw ExecutionError::EMPTY_HAND;

  if (state.reg.hand == 0) {
    state.cursor = param - 1;
    is_successful_jump = true;
  }
}

/*
 * engine.h - Core game logic, destroyed after each level ends
 */

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

// engine.cpp
GameEngine::GameEngine(const LevelData &data) : state(data), level_data(data) {}
void GameEngine::loadProgram(const Program &new_program) {
  program = new_program;
  executor.program_size = program.size();
}
const GameState &GameEngine::getState() const { return state; }
bool GameEngine::validateOutput() const {
  return state.outbox_buffer == level_data.expected_outbox;
}
bool GameEngine::executeNextInstruction() {
  if (state.cursor >= program.size()) {
    return false;
  }

  const Instruction current = program.at(state.cursor);

  if (current.type == InstructionType::INBOX &&
      state.inbox_cursor >= state.inbox.size()) {
    return false;
  }

  executor.executeInstruction(state, current);
  return true;
}

/*
 * UI.h - User interface - Further development needed
 */

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

// UI.cpp
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
