#include "executor.hpp"

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
