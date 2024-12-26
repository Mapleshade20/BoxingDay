#include "engine.hpp"

GameEngine::GameEngine(const LevelData &data)
    : state(data), level_data(data), steps(0) {}

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
      state.inbox_cursor >= int(state.inbox.size())) {
    return false;
  }

  executor.executeInstruction(state, current);
  steps++;
  return true;
}

int GameEngine::getSteps() const { return steps; }

int GameEngine::getProgramLength() const { return program.size(); }
