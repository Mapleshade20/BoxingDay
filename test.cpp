#include "parse_gamedata.h"
#include <iostream>

void PrintGameData(const GameData &game_data) {
  std::cout << "Level: " << game_data.level << std::endl;
  std::cout << "Title: " << game_data.title << std::endl;
  std::cout << "Description: " << game_data.desc << std::endl;

  std::cout << "Outbox: ";
  for (int i = 0; i < 256 && game_data.outbox[i] != 0; ++i) {
    std::cout << game_data.outbox[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Inbox: ";
  for (int i = 0; i < 256 && game_data.inbox[i] != 0; ++i) {
    std::cout << game_data.inbox[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Available Instructions: ";
  for (int i = 0; i < 8; ++i) {
    if (!game_data.available_instructions[i].empty()) {
      std::cout << game_data.available_instructions[i] << " ";
    }
  }
  std::cout << std::endl;

  std::cout << "Available Tiles: " << game_data.available_tiles << std::endl;
}

int main() {
  std::string path = "./data/game_1.toml";
  auto result = ParseGameData(path);

  if (!result.second) {
    std::cerr << "Failed to parse game data from " << path << std::endl;
    return 1;
  }

  PrintGameData(result.first);
  return 0;
}
