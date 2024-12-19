#pragma once

#include <string>
#include <vector>

#include "../core/leveldata.hpp"
#include "../core/types.hpp"
#include "renderer.hpp"

class PickerState {
private:
  std::vector<bool> is_param_complete;

public:
  std::vector<int> cursor_positions = {
      1, 1, 1};  // 记录每层tab指针位置，从而记录每次操作
  int current_tab = 1;
  int command_length = 1;
  int total_tiles;
  std::vector<InstructionType> instructions;
  std::vector<int> instruction_params;
  std::vector<InstructionType> valid_instructions;

  // Constructor for PickerState
  PickerState(std::vector<InstructionType> valid_instructions, int total_tiles);

  // Move cursor up or down
  void moveCursor(bool up, int max_length);

  // Move tab left or right
  void switchTab(bool left);

  // Create a new command
  void addCommand(bool above);

  // Delete a command
  void removeCommand();

  // Save the current command
  void saveCurrentCommand();

  // Get the length of the current command
  int getCommandLength();

  // Check if all parameters are complete
  bool areAllParamsComplete();
};

class PickerRenderer {
private:
  Renderer renderer;

public:
  // Constructor for PickerRenderer
  PickerRenderer(int start_x, int start_y);

  // Convert InstructionType to string
  static std::string insToString(InstructionType instruction_type);

  // Render parameters
  void renderParameters(PickerState &state);

  // Render operations
  void renderOperations(int length, std::vector<InstructionType> instructions,
                        std::vector<int> instruction_params);
};

class PickerInteract {
private:
  PickerState state;
  PickerRenderer renderer;

public:
  // Constructor for PickerInteract
  PickerInteract(int start_x, int start_y, const LevelData &l);

  // Interact with the picker
  Program interact();
};
