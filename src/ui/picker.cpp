#include "picker.hpp"

#include <unistd.h>

#include <iostream>

#include "libtui.hpp"

// Constructor for PickerRenderer
PickerRenderer::PickerRenderer(int start_x, int start_y)
    : renderer(start_x, start_y) {}

// Convert InstructionType to string
std::string PickerRenderer::insToString(InstructionType instruction_type) {
  switch (instruction_type) {
    case InstructionType::INBOX:
      return ("inbox");
    case InstructionType::OUTBOX:
      return ("outbox");
    case InstructionType::ADD:
      return ("add");
    case InstructionType::SUB:
      return ("sub");
    case InstructionType::COPYFROM:
      return ("copyfrom");
    case InstructionType::COPYTO:
      return ("copyto");
    case InstructionType::JUMP:
      return ("jump");
    case InstructionType::JUMPIFZERO:
      return ("jumpifzero");
    default:
      return ("error");
  }
}

// Render parameters
void PickerRenderer::renderParameters(PickerState &state) {
  int current_tab = state.current_tab;
  int command_length = state.command_length;
  int total_tiles = state.total_tiles;
  std::vector<int> cursor_positions = state.cursor_positions;
  std::vector<Instruction> instructions = state.instructions;
  std::vector<InstructionType> valid_instructions = state.valid_instructions;
  switch (current_tab) {
    case 0:
      renderer.renderWord(0, cursor_positions[0], ">");
      for (int i = 1; i <= command_length; i++) {
        renderer.renderWord(1, i, std::to_string(i));
        renderer.renderWord(
            6, i, PickerRenderer::insToString(instructions[i - 1].type));
        if (instructions[i - 1].param > -1) {
          renderer.renderWord(19, i, std::to_string(instructions[i - 1].param));
          break;
        }
      }
      break;
    case 1:
      renderer.renderWord(0, cursor_positions[0], "-");
      for (int i = 1; i <= command_length; i++) {
        renderer.renderWord(1, i, std::to_string(i));
      }
      renderer.renderWord(5, cursor_positions[1], ">");
      for (int i = 1; i <= valid_instructions.size(); i++) {
        renderer.renderWord(
            6, i, PickerRenderer::insToString(valid_instructions[i - 1]));
      }
      break;
    case 2:
      renderer.renderWord(0, cursor_positions[0], "-");
      renderer.renderWord(5, cursor_positions[1], "-");
      renderer.renderWord(18, cursor_positions[2], ">");
      for (int i = 1; i <= command_length; i++) {
        renderer.renderWord(1, i, std::to_string(i));
        renderer.renderWord(
            6, i, PickerRenderer::insToString(instructions[i - 1].type));
      }
      {
        InstructionType tmp = valid_instructions[cursor_positions[1] - 1];
        if (tmp == InstructionType::JUMP ||
            tmp == InstructionType::JUMPIFZERO) {
          for (int i = 1; i <= command_length + 1; i++) {
            renderer.renderWord(19, i, std::to_string(i));
          }
        } else {
          for (int i = 1; i <= valid_instructions.size(); i++) {
            renderer.renderWord(19, i, std::to_string(i - 1));
          }
        }
      }
      break;
    default:
      break;
  }
}
void PickerRenderer::renderOperations(int cursor_position) {
  renderer.clearArea(0, 1, 1, MAX_INS + 1);
  renderer.renderWord(0, cursor_position, ">");
}
// Constructor for PickerState
PickerState::PickerState(std::vector<InstructionType> valid_instructions,
                         int total_tiles) {
  this->valid_instructions = valid_instructions;  // GameUI
  this->total_tiles = total_tiles;
}

// Move cursor up or down
void PickerState::moveCursor(bool up, int max_length) {
  if (up) {
    if (cursor_positions[current_tab] > 1) cursor_positions[current_tab]--;
  } else {
    if (cursor_positions[current_tab] < max_length)
      cursor_positions[current_tab]++;
  }
  for (int i = current_tab + 1; i <= 2; i++) cursor_positions[i] = 1;
}

// Move tab left or right
void PickerState::switchTab(bool left) {
  if (left) {
    if (current_tab > 0) current_tab--;
  } else {
    if (current_tab == 0) {
      current_tab++;
      cursor_positions[current_tab] = 1;
      return;
    }
    int cur = cursor_positions[1] - 1;
    if (current_tab < 2 && instructions[cur].type != InstructionType::INBOX &&
        instructions[cur].type != InstructionType::OUTBOX) {
      current_tab++;
      cursor_positions[current_tab] = 1;
    } else {
      PickerState::saveCurrentCommand();
      current_tab = 0;
    }
  }
}

// Create a new command
void PickerState::addCommand(bool above, const int max_ins) {
  if (current_tab > 0 || command_length >= max_ins) return;
  int cur = cursor_positions[0];  // create a new param, then move tab(right)
  command_length++;
  if (above) {
    instructions.insert(instructions.begin() + cur - 1,
                        {InstructionType::ERROR, -1});
    is_param_complete.insert(is_param_complete.begin() + cur - 1, false);
  } else {
    instructions.insert(instructions.begin() + cur,
                        {InstructionType::ERROR, -1});
    is_param_complete.insert(is_param_complete.begin() + cur, false);
    cursor_positions[0]++;
  }
  cursor_positions[1] = 1;
  cursor_positions[2] = 1;
  PickerState::switchTab(0);
}

// Delete a command
void PickerState::removeCommand() {
  if (current_tab > 0) return;
  int cur = cursor_positions[0];
  command_length--;
  instructions.erase(instructions.begin() + cur - 1);
  is_param_complete.erase(is_param_complete.begin() + cur - 1);
  for (int i = 0; i < command_length; i++) {
    if ((instructions[i].type == InstructionType::JUMP ||
         instructions[i].type == InstructionType::JUMPIFZERO) &&
        instructions[i].param >= cur) {
      instructions[i].param--;
      if (instructions[i].param == 0) instructions[i].param++;
    }
  }
}

// Save the current command
void PickerState::saveCurrentCommand() {
  int cur = cursor_positions[0] - 1;
  is_param_complete[cur] = true;
  instructions[cur].type = valid_instructions[cursor_positions[1] - 1];
  if (instructions[cur].type == InstructionType::INBOX ||
      instructions[cur].type == InstructionType::OUTBOX) {
    instructions[cur].param = -1;
    return;
  }
  if (instructions[cur].type == InstructionType::JUMP ||
      instructions[cur].type == InstructionType::JUMPIFZERO) {
    instructions[cur].param = cursor_positions[2];
    return;
  }
  instructions[cur].param = cursor_positions[2] - 1;
}

// Get the length of the current command
int PickerState::getCommandLength() {
  if (current_tab == 0) return command_length;
  if (current_tab == 2) {
    InstructionType tmp = valid_instructions[cursor_positions[1] - 1];
    if (tmp == InstructionType::JUMP || tmp == InstructionType::JUMPIFZERO)
      return command_length + 1;
    return total_tiles;
  }
  return valid_instructions.size();
}

// Check if all parameters are complete
bool PickerState::areAllParamsComplete() {
  for (int i = 0; i < command_length; i++) {
    if (is_param_complete[i] == false) return false;
  }
  return true;
}

// Constructor for PickerInteract
PickerInteract::PickerInteract(int start_x, int start_y, const LevelData &l)
    : state(l.available_instructions, l.available_tiles),
      renderer(start_x, start_y) {}

// Interact with the picker
Program PickerInteract::interact() {
  bool is_running = true;
  state.addCommand(true, renderer.MAX_INS);
  while (is_running) {
    usleep(1000000 / 60);
    if (kbhit()) {
      char input;
      std::cin >> input;
      switch (input) {
        case 'h':
          state.switchTab(true);
          break;
        case 'l':
          state.switchTab(false);
          break;
        case 'j':
          state.moveCursor(false, state.getCommandLength());
          break;
        case 'k':
          state.moveCursor(true, state.getCommandLength());
          break;
        case 'd':
          state.removeCommand();
          break;
        case 'o':
          state.addCommand(false, renderer.MAX_INS);
          break;
        case 'O':
          state.addCommand(true, renderer.MAX_INS);
          break;
        case '\n':
          state.switchTab(false);
          break;
        case 'R':
          if (state.areAllParamsComplete()) is_running = false;
          break;
        default:
          break;
      }
    }
    renderer.renderParameters(state);
  }
  state.switchTab(true);
  state.switchTab(true);
  state.cursor_positions = {1, 1, 1};
  renderer.renderParameters(state);
  Program program;
  program.setInstructions(this->state.instructions);
  return program;
}
