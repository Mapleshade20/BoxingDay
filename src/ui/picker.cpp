#include <iostream>
#include <string>
#include <vector>

#include "../core/types.hpp"
#include "./libtui.hpp"
#include "renderer.hpp"

class PickerState {
private:
  std::vector<bool> complete_param;

public:
  std::vector<int> cursor = {1, 1,
                             1};  // 记录每层tab指针的位置，从而记录每次的操作
  int tab = 1;
  int len = 1;  // 命令长度
  int tile_num;
  std::vector<InstructionType> param;
  std::vector<int> param_num;
  std::vector<InstructionType> valid_instructions;
  PickerState(std::vector<InstructionType> valid_ins, int num);
  void move1(bool up, int len_tab);
  void moveTab(bool left);
  void newCommand(bool up);
  void deleteCommand();
  void saveCommand();
  int getLen();
  bool checkParam();
};

class PickerRenderer {
  Renderer renderer;

public:
  static std::string insToString(InstructionType instruction_type);
  void paramRender(PickerState &state);
  void operateRender(int len, std::vector<InstructionType> param,
                     std::vector<int> param_num);
};
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
void PickerRenderer::paramRender(PickerState &state) {
  int tab = state.tab;
  int len = state.len;
  int tile_num = state.tile_num;
  std::vector<int> cursor = state.cursor;
  std::vector<InstructionType> param = state.param;
  std::vector<int> param_num = state.param_num;
  std::vector<InstructionType> valid_instructions = state.valid_instructions;
  switch (tab) {
    case 0:
      renderer.renderWord(31, cursor[0], ">");
      for (int i = 1; i <= len; i++) {
        renderer.renderWord(32, i, std::to_string(i));
        renderer.renderWord(37, i, PickerRenderer::insToString(param[i - 1]));
        if (param_num[i - 1] > -1) {
          renderer.renderWord(50, i, std::to_string(param_num[i - 1]));
          break;
        }
      }
      break;
    case 1:
      renderer.renderWord(31, cursor[0], "-");
      for (int i = 1; i <= len; i++) {
        renderer.renderWord(32, i, std::to_string(i));
      }
      renderer.renderWord(36, cursor[1], ">");
      for (int i = 1; i <= valid_instructions.size(); i++) {
        renderer.renderWord(
            37, i, PickerRenderer::insToString(valid_instructions[i - 1]));
      }
      break;
    case 2:
      renderer.renderWord(31, cursor[0], "-");
      renderer.renderWord(36, cursor[1], "-");
      renderer.renderWord(49, cursor[2], ">");
      for (int i = 1; i <= len; i++) {
        renderer.renderWord(32, i, std::to_string(i));
        renderer.renderWord(37, i, PickerRenderer::insToString(param[i - 1]));
      }
      {
        InstructionType tmp = valid_instructions[cursor[1] - 1];
        if (tmp == InstructionType::JUMP ||
            tmp == InstructionType::JUMPIFZERO) {
          for (int i = 1; i <= len + 1; i++) {
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

PickerState::PickerState(std::vector<InstructionType> valid_ins, int num) {
  valid_instructions = valid_ins;  // GameUI
  tile_num = num;
}
void PickerState::move1(bool up, int len_tab) {  // to move up or down
  if (up) {
    if (cursor[tab] > 1) cursor[tab]--;
  } else {
    if (cursor[tab] < len_tab) cursor[tab]++;
  }
}
void PickerState::moveTab(bool left) {  // to move tab(left or right)
  if (left) {
    if (tab > 0) tab--;
  } else {
    if (tab == 0) {
      tab++;
      cursor[tab] = 1;
      return;
    }
    int cur = cursor[1] - 1;
    if (tab < 2 && param[cur] != InstructionType::INBOX &&
        param[cur] != InstructionType::OUTBOX) {
      tab++;
      cursor[tab] = 1;
    } else {
      PickerState::saveCommand();
      tab = 0;
    }
  }
}
void PickerState::newCommand(bool up) {
  if (tab > 0) return;
  int cur = cursor[0];  // to create a new param, then move tab(right)
  len++;
  if (up) {
    param.insert(param.begin() + cur - 1, InstructionType::ERROR);
    param_num.insert(param_num.begin() + cur - 1, -1);
    complete_param.insert(complete_param.begin() + cur - 1, false);
  } else {
    param.insert(param.begin() + cur, InstructionType::ERROR);
    param_num.insert(param_num.begin() + cur, -1);
    complete_param.insert(complete_param.begin() + cur, false);
    cursor[0]++;
  }
  PickerState::moveTab(0);
}
void PickerState::deleteCommand() {  // to delete a param
  if (tab > 0) return;
  int cur = cursor[0];
  len--;
  param.erase(param.begin() + cur - 1);
  param_num.erase(param_num.begin() + cur - 1);
  complete_param.erase(complete_param.begin() + cur - 1);
  for (int i = 0; i < len; i++) {
    if ((param[i] == InstructionType::JUMP ||
         param[i] == InstructionType::JUMPIFZERO) &&
        param_num[i] >= cur) {
      param_num[i]--;
      if (param_num[i] == 0) param_num[i]++;
    }
  }
}
void PickerState::saveCommand() {
  int cur = cursor[0] - 1;
  complete_param[cur] = true;
  param[cur] = valid_instructions[cursor[1] - 1];
  if (param[cur] == InstructionType::INBOX ||
      param[cur] == InstructionType::OUTBOX) {
    param_num[cur] = -1;
    return;
  }
  if (param[cur] == InstructionType::JUMP ||
      param[cur] == InstructionType::JUMPIFZERO) {
    param_num[cur] = cursor[2];
    return;
  }
  param_num[cur] = cursor[2] - 1;
}
int PickerState::getLen() {
  if (tab == 0) return len;
  if (tab == 2) {
    InstructionType tmp = valid_instructions[cursor[1] - 1];
    if (tmp == InstructionType::JUMP || tmp == InstructionType::JUMPIFZERO)
      return len + 1;
    return tile_num;
  }
  return valid_instructions.size();
}
bool PickerState::checkParam() {
  for (int i = 0; i < len; i++) {
    if (complete_param[i] == false) return false;
  }
  return true;
}
class PickerInteract {
private:
  PickerState state;
  PickerRenderer renderer;

public:
  void interact();
};
void PickerInteract::interact() {
  bool flag = true;
  state.newCommand(true);
  while (flag) {
    if (kbhit()) {
      char input;
      std::cin >> input;
      switch (input) {
        case 'h':
          state.moveTab(true);
          break;
        case 'l':
          state.moveTab(false);
          break;
        case 'j':
          state.move1(false, state.getLen());
          break;
        case 'k':
          state.move1(true, state.getLen());
          break;
        case 'd':
          state.deleteCommand();
          break;
        case 'o':
          state.newCommand(false);
          break;
        case 'O':
          state.newCommand(true);
          break;
        case '\n':
          state.moveTab(false);
          break;
        case 'R':
          if (state.checkParam()) flag = false;
        default:
          break;
      }
    }
    renderer.paramRender(state);
  }
}