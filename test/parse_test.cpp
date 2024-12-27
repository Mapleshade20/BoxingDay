#include <iostream>

#include "../src/fileIO/level_reader.hpp"

void PrintGameData(const LevelData &game_data) {
  std::cout << "Level: " << game_data.level << std::endl;
  std::cout << "Title: " << game_data.title << std::endl;
  std::cout << "Description: " << game_data.description << std::endl;

  std::cout << "Outbox: ";
  for (auto item : game_data.expected_outbox) {
    std::cout << item << " ";
  }
  std::cout << std::endl;

  std::cout << "Inbox: ";
  for (auto item : game_data.initial_inbox) {
    std::cout << item << " ";
  }
  std::cout << std::endl;

  std::cout << "Available Tiles: " << game_data.available_tiles << std::endl;
}

int main() {
  std::string path = "./data/game_1.toml";
  auto result = ParseLevelData(path);

  if (!result.second) {
    std::cerr << "Failed to parse game data from " << path << std::endl;
    return 1;
  }

  PrintGameData(result.first);
  return 0;
}
