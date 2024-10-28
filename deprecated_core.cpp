#include <iostream>
// #include <sstream>
#include <string>
#include <vector>

using namespace std;

constexpr int MAX_INSTRUCTIONS = 128;
constexpr int MAX_INBOX = 256;
constexpr int MAX_OUTBOX = 256;

struct Instruction {
  std::string name; // 指令类型
  int param;        // 操作位置
  // bool is_box;      // 判断是否需要读取param
};
struct GameData {
  int level;
  std::string title;
  std::string desc;
  int outbox_size;
  int outbox[MAX_OUTBOX];
  int inbox_size;
  int inbox[MAX_INBOX];
  std::string available_instructions[8];
  int available_tiles;
};

struct Register {
  int current_tile;
  int dest_tile;
  int hand;
  bool is_empty;
};

struct Tile {
  int value;
  bool is_empty;
};

// TODO: Gamedata should be passed in as a parameter from parser module
const GameData game_data[3] = {
    {1, // level 1
     "One By One",
     "Move each number from inbox to outbox.",
     2,
     {1, 2},
     2,
     {1, 2},
     {"inbox", "outbox"},
     0},
    {2, // level 2
     "Subtract You and Me",
     "For every two numbers, subtract A from B and subtract B from A. Put the "
     "results in outbox.",
     8,
     {3, 9, 5, 1, -2, -2, 9, -9},
     8,
     {-6, 6, 4, -4, 0, 0, 18, -18},
     {"inbox", "outbox", "copyto", "copyfrom", "add", "sub", "jump",
      "jumpifzero"},
     3},
    {3, // level 3
     "Verify Identity",
     "For every two numbers, put one of them in outbox if they're identical. "
     "Otherwise discard them.",
     8,
     {6, 2, 7, 7, -9, 3, -3, -3},
     2,
     {7, -3},
     {"inbox", "outbox", "copyto", "copyfrom", "add", "sub", "jump",
      "jumpifzero"},
     3}};

// TODO: Make the main function a standalone "core" function
// So it should accept `program` and `gamedata` structs as parameters
// And return the final results to the UI handler

int main() {

  // TODO: Change `program` initialization -> user input

  std::vector<Instruction> program(MAX_INSTRUCTIONS);
  program[0].name = "inbox";
  program[1].name = "outbox";
  program[2].name = "inbox";
  program[3].name = "outbox";
  // program[4].name = "end";

  // Initialize game variables
  int cursor = 0; // 指令位置
  bool err_flag = false;
  int err_pos = 0;
  Register reg = {0, 0, 0, true}; // 机器人状态
  int available_tiles = game_data[0].available_tiles;
  vector<Tile> tiles(available_tiles + 1);
  string valid_names[8];
  for (int i = 0; i < 8; i++)
    valid_names[i] = game_data[0].available_instructions[i];

  vector<int> inbox(game_data[0].inbox_size);
  int inbox_cursor = 0; // inbox指针
  for (int i = 0; i < MAX_INBOX; i++) {
    inbox[i] = game_data[0].inbox[i];
  }

  vector<int> outbox_buffer; // 玩家实际outbox

  vector<int> outbox_expected(game_data[0].outbox_size);
  for (int i = 0; i < MAX_OUTBOX; i++) {
    outbox_expected[i] = game_data[0].outbox[i];
  }

  // Main loop
  while (cursor < program.size()) {

    // cout << "DEBUG: checking program[" << cursor
    // << "].name: " << program[cursor].name << endl;

    if (program[cursor].name == "end") {
      break;
    }
    bool valid_flag = false;

    for (int i = 0; i < 8; i++) {
      if (program[cursor].name == valid_names[i]) {
        valid_flag = true;
        break;
      }
    }

    if (!valid_flag) {
      err_flag = true;
      err_pos = cursor;
      break;

    } else if (program[cursor].name == "inbox") {

      if (inbox_cursor == inbox.size())
        break;
      reg.dest_tile = -1;
      // animation
      reg.current_tile = reg.dest_tile;
      reg.is_empty = false;
      reg.hand = inbox[inbox_cursor];
      inbox_cursor++;

    } else if (program[cursor].name == "outbox") {

      if (reg.is_empty) {
        err_flag = true;
        err_pos = cursor;
        break;
      }
      reg.dest_tile = -2;
      // animation
      reg.current_tile = reg.dest_tile;
      outbox_buffer.push_back(reg.hand);
      reg.is_empty = false;

    } else if (program[cursor].name == "add") {

      if (reg.is_empty || program[cursor].param >= available_tiles ||
          tiles[program[cursor].param].is_empty) {
        err_flag = true;
        err_pos = cursor;
        break;
      }
      reg.dest_tile = program[cursor].param;
      // animation
      reg.current_tile = reg.dest_tile;
      reg.hand += tiles[program[cursor].param].value;
    } else if (program[cursor].name == "sub") {

      if (reg.is_empty || program[cursor].param >= available_tiles ||
          tiles[program[cursor].param].is_empty) {
        err_flag = true;
        err_pos = cursor;
        break;
      }
      reg.dest_tile = program[cursor].param;
      // animation
      reg.current_tile = reg.dest_tile;
      reg.hand -= tiles[program[cursor].param].value;

    } else if (program[cursor].name == "copyto") {

      if (reg.is_empty || program[cursor].param >= available_tiles) {
        err_flag = true;
        err_pos = cursor;
        break;
      }
      reg.dest_tile = program[cursor].param;
      // animation
      reg.current_tile = reg.dest_tile;
      tiles[program[cursor].param].value = reg.hand;
      tiles[program[cursor].param].is_empty = false;

    } else if (program[cursor].name == "copyfrom") {

      if (program[cursor].param >= available_tiles ||
          tiles[program[cursor].param].is_empty) {
        err_flag = true;
        err_pos = cursor;
        break;
      }
      reg.dest_tile = program[cursor].param;
      // animation
      reg.current_tile = reg.dest_tile;
      reg.is_empty = false;
      reg.hand = tiles[program[cursor].param].value;

    } else if (program[cursor].name == "jump") {

      if (program[program[cursor].param].name == "") {
        err_flag = true;
        err_pos = cursor;
        break;
      }
      cursor = program[cursor].param;
      continue;

    } else if (program[cursor].name == "jumpifzero") {

      if (reg.is_empty || program[program[cursor].param].name == "") {
        err_flag = true;
        err_pos = cursor;
        break;
      } // 当前积木不为零但指令不存在，这里认为也是运行error
      if (reg.hand == 0) {
        cursor = program[cursor].param;
        continue;
      }

    } else
      break;

    cursor++;
  }

  // Evaluate the result
  bool success = true;
  if (err_flag) {
    cout << "Error on instruction " << err_pos + 1 << endl;
    return 0;
  }
  if (outbox_buffer.size() == outbox_expected.size()) {
    for (int i = 0; i < outbox_expected.size(); i++) {
      if (outbox_buffer[i] != outbox_expected[i]) {
        success = false;
        break;
      }
    }
  } else {
    success = false;
  }
  if (success) {
    cout << "Success" << endl;
  } else {
    cout << "Fail" << endl;
  }

  return 0;
}
