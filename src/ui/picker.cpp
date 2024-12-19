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
  std::vector<InstructionType> instructions = state.instructions;
  std::vector<int> instruction_params = state.instruction_params;
  std::vector<InstructionType> valid_instructions = state.valid_instructions;
  switch (current_tab) {
    case 0:
      renderer.renderWord(31, cursor_positions[0], ">");
      for (int i = 1; i <= command_length; i++) {
        renderer.renderWord(32, i, std::to_string(i));
        renderer.renderWord(37, i,
                            PickerRenderer::insToString(instructions[i - 1]));
        if (instruction_params[i - 1] > -1) {
          renderer.renderWord(50, i, std::to_string(instruction_params[i - 1]));
          break;
        }
      }
      break;
    case 1:
      renderer.renderWord(31, cursor_positions[0], "-");
      for (int i = 1; i <= command_length; i++) {
        renderer.renderWord(32, i, std::to_string(i));
      }
      renderer.renderWord(36, cursor_positions[1], ">");
      for (int i = 1; i <= valid_instructions.size(); i++) {
        renderer.renderWord(
            37, i, PickerRenderer::insToString(valid_instructions[i - 1]));
      }
      break;
    case 2:
      renderer.renderWord(31, cursor_positions[0], "-");
      renderer.renderWord(36, cursor_positions[1], "-");
      renderer.renderWord(49, cursor_positions[2], ">");
      for (int i = 1; i <= command_length; i++) {
        renderer.renderWord(32, i, std::to_string(i));
        renderer.renderWord(37, i,
                            PickerRenderer::insToString(instructions[i - 1]));
      }
      {
        InstructionType tmp = valid_instructions[cursor_positions[1] - 1];
        if (tmp == InstructionType::JUMP ||
            tmp == InstructionType::JUMPIFZERO) {
          for (int i = 1; i <= command_length + 1; i++) {
            renderer.renderWord(50, i, std::to_string(i));
          }
        } else {
          for (int i = 1; i <= valid_instructions.size(); i++) {
            renderer.renderWord(50, i, std::to_string(i - 1));
          }
        }
      }
      break;
    default:
      break;
  }
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
    if (current_tab < 2 && instructions[cur] != InstructionType::INBOX &&
        instructions[cur] != InstructionType::OUTBOX) {
      current_tab++;
      cursor_positions[current_tab] = 1;
    } else {
      PickerState::saveCurrentCommand();
      current_tab = 0;
    }
  }
}

// Create a new command
void PickerState::addCommand(bool above) {
  if (current_tab > 0) return;
  int cur = cursor_positions[0];  // create a new param, then move tab(right)
  command_length++;
  if (above) {
    instructions.insert(instructions.begin() + cur - 1, InstructionType::ERROR);
    instruction_params.insert(instruction_params.begin() + cur - 1, -1);
    is_param_complete.insert(is_param_complete.begin() + cur - 1, false);
  } else {
    instructions.insert(instructions.begin() + cur, InstructionType::ERROR);
    instruction_params.insert(instruction_params.begin() + cur, -1);
    is_param_complete.insert(is_param_complete.begin() + cur, false);
    cursor_positions[0]++;
  }
  PickerState::switchTab(0);
}

// Delete a command
void PickerState::removeCommand() {
  if (current_tab > 0) return;
  int cur = cursor_positions[0];
  command_length--;
  instructions.erase(instructions.begin() + cur - 1);
  instruction_params.erase(instruction_params.begin() + cur - 1);
  is_param_complete.erase(is_param_complete.begin() + cur - 1);
  for (int i = 0; i < command_length; i++) {
    if ((instructions[i] == InstructionType::JUMP ||
         instructions[i] == InstructionType::JUMPIFZERO) &&
        instruction_params[i] >= cur) {
      instruction_params[i]--;
      if (instruction_params[i] == 0) instruction_params[i]++;
    }
  }
}

// Save the current command
void PickerState::saveCurrentCommand() {
  int cur = cursor_positions[0] - 1;
  is_param_complete[cur] = true;
  instructions[cur] = valid_instructions[cursor_positions[1] - 1];
  if (instructions[cur] == InstructionType::INBOX ||
      instructions[cur] == InstructionType::OUTBOX) {
    instruction_params[cur] = -1;
    return;
  }
  if (instructions[cur] == InstructionType::JUMP ||
      instructions[cur] == InstructionType::JUMPIFZERO) {
    instruction_params[cur] = cursor_positions[2];
    return;
  }
  instruction_params[cur] = cursor_positions[2] - 1;
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
  state.addCommand(true);
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
          state.addCommand(false);
          break;
        case 'O':
          state.addCommand(true);
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

  Program program;
  program.setInstructions(this->state.instructions);
  // FIXME: Refactor state.instructions to be a vector<Instruction>
  return program;
}
