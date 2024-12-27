// This file contains the function that parses the game data from a toml file.

/* TOML file format:
[info]
level = 1
title = "One By One"
desc = "Put each number in the outbox, one by one."

[game]
outbox = [1, 2]
inbox = [1, 2]
available_instructions = ["inbox", "outbox"]
available_tiles = 0
*/

#include "level_reader.hpp"

#include <exception>
#include <stdexcept>

#include "../../include/toml.hpp"
#include "../core/types.hpp"

std::pair<LevelData, bool> ParseLevelData(std::string &path) {
  LevelData data;
  toml::value table;

  try {
    table = toml::parse(path);

    data.level = table.at("info").at("level").as_integer();
    if (data.level < 1) {
      throw std::runtime_error("Invalid level number");
    }
    data.title = table.at("info").at("title").as_string();
    data.description = table.at("info").at("desc").as_string();

    auto outbox_array = table.at("game").at("outbox").as_array();
    for (auto item : outbox_array) {
      data.expected_outbox.push_back(item.as_integer());
    }

    auto inbox_array = table.at("game").at("inbox").as_array();
    for (auto item : inbox_array) {
      data.initial_inbox.push_back(item.as_integer());
    }

    auto instructions_array =
        table.at("game").at("available_instructions").as_array();
    for (auto item : instructions_array) {
      data.available_instructions.push_back(
          Instruction::fromString(item.as_string()));
    }

    data.available_tiles = table.at("game").at("available_tiles").as_integer();

  } catch (const std::exception &err) {
    return std::make_pair(data, false);
  }

  return std::make_pair(data, true);
}
