// This file contains the function that parses the game data from a toml file.

#include "parse_gamedata.h"

std::pair<GameData, bool> ParseGameData(std::string path) {
  GameData game_data;
  toml::table tbl;
  try {
    tbl = toml::parse_file(path);
  } catch (const toml::parse_error &err) {
    return std::make_pair(game_data, false);
  }

  try {
    game_data.level = tbl["level"].value_or(0);
    if (game_data.level < 1 || game_data.level > 60) {
      throw std::runtime_error("Invalid level number.");
    }
    game_data.title = tbl["title"].value_or("");
    game_data.desc = tbl["desc"].value_or("");

    auto outbox_array = tbl["game"]["outbox"].as_array();
    if (outbox_array) {
      for (size_t i = 0; i < outbox_array->size() && i < 256; ++i) {
        game_data.outbox[i] = outbox_array->at(i).value_or(0);
      }
    } else {
      throw std::runtime_error("Invalid outbox array.");
    }

    auto inbox_array = tbl["game"]["inbox"].as_array();
    if (inbox_array) {
      for (size_t i = 0; i < inbox_array->size() && i < 256; ++i) {
        game_data.inbox[i] = inbox_array->at(i).value_or(0);
      }
    } else {
      throw std::runtime_error("Invalid inbox array.");
    }

    auto instructions_array = tbl["game"]["available_instructions"].as_array();
    if (instructions_array) {
      for (size_t i = 0; i < instructions_array->size() && i < 8; ++i) {
        game_data.available_instructions[i] =
            instructions_array->at(i).value_or("");
      }
    } else {
      throw std::runtime_error("Invalid available_instructions array.");
    }
    game_data.available_tiles = tbl["game"]["available_tiles"].value_or(0);
  } catch (const std::runtime_error &err) {
    return std::make_pair(game_data, false);
  }

  return std::make_pair(game_data, true);
}
