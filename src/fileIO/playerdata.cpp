#include "playerdata.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>

DataManager::DataManager() {
  this->home_dir = std::getenv("HOME");
  this->filename = home_dir + "/Documents/BoxingDay/gameplay.dat";
}
/*
int DataManager::readPassedLevels() {
  std::ifstream file(filename, std::ios::binary);
  if (file.is_open()) {
    int size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    file.close();
    return size;
  } else {
    return 0;
  }
}
*/
std::vector<PlayerData> DataManager::readLevelData() {
  std::vector<PlayerData> data;
  std::ifstream file(filename, std::ios::binary);
  if (file.is_open()) {
    int size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    data.resize(size);
    for (int i = 0; i < size; i++) {
      file.read(reinterpret_cast<char*>(&data[i]), sizeof(PlayerData));
    }
    file.close();
    return data;
  } else {
    return {};
  }
}

void DataManager::writeData(int level, int instructions, int steps) {
  std::vector<PlayerData> data;
  data = readLevelData();
  if (data.size() < level) {
    data.push_back({level, -1, -1});
  }
  if (data[level - 1].min_instructions == -1) {
    data[level - 1] = {level, instructions, steps};
  } else {
    if (instructions < data[level - 1].min_instructions)
      data[level - 1].min_instructions = instructions;
    if (steps < data[level - 1].min_steps) data[level - 1].min_steps = steps;
  }
  std::ofstream file(filename, std::ios::binary);
  if (file.is_open()) {
    int size = data.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& i : data) {
      file.write(reinterpret_cast<const char*>(&i), sizeof(PlayerData));
    }
    file.close();
  } else {
    std::cerr << "error" << std::endl;
  }
}
