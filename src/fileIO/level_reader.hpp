#pragma once

#include <string>
#include <utility>

#include "../core/leveldata.hpp"

std::pair<LevelData, bool> ParseLevelData(std::string &path);
