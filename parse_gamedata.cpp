// This file contains the function that parses the game data from a toml file.

#include "parse_gamedata.h"

std::pair<GameData, bool> ParseGameData(std::string path) {
  GameData game_data;
  toml::value tbl;
  std::fill(std::begin(game_data.outbox), std::end(game_data.outbox), 0);
  std::fill(std::begin(game_data.inbox), std::end(game_data.inbox), 0);

  try {
    tbl = toml::parse(path);
  } catch (const std::exception &err) {
    return std::make_pair(game_data, false);
  }

  try {
    game_data.level = tbl.at("level").as_integer();
    // if (game_data.level < 1 || game_data.level > 60) {
    // throw std::runtime_error("Invalid level number.");
    // }
    game_data.title = tbl.at("title").as_string();
    game_data.desc = tbl.at("desc").as_string();

    auto outbox_array = tbl.at("game").at("outbox").as_array();
    for (size_t i = 0; i < outbox_array.size() && i < 256; ++i) {
      game_data.outbox[i] = outbox_array[i].as_integer();
    }

    auto inbox_array = tbl.at("game").at("inbox").as_array();
    for (size_t i = 0; i < inbox_array.size() && i < 256; ++i) {
      game_data.inbox[i] = inbox_array[i].as_integer();
    }

    auto instructions_array =
        tbl.at("game").at("available_instructions").as_array();
    for (size_t i = 0; i < instructions_array.size() && i < 8; ++i) {
      game_data.available_instructions[i] = instructions_array[i].as_string();
    }

    game_data.outbox_size = tbl.at("game").at("outbox_size").as_integer();
    game_data.inbox_size = tbl.at("game").at("inbox_size").as_integer();
    game_data.available_tiles =
        tbl.at("game").at("available_tiles").as_integer();

  } catch (const std::runtime_error &err) {
    return std::make_pair(game_data, false);
  }

  return std::make_pair(game_data, true);
}
