#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct PlayerData {
  int level;
  int min_instructions;
  int min_steps;
};

class DataManager {
private:
  std::string homeDirectory = std::getenv("HOME");
  std::string filename = homeDirectory + "/Documents/BoxingDay/gameplay.dat";

public:
  int readPassedLevels();
  std::vector<PlayerData> readLevelData(int level);
  void writeData(int level, int instructions, int steps);
};

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

std::vector<PlayerData> DataManager::readLevelData(int level) {
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
    if (size < level) {
      data.push_back({level, -1, -1});
    }
    return data;
  } else {
    return {{level, -1, -1}};
  }
}

void DataManager::writeData(int level, int instructions, int steps) {
  std::vector<PlayerData> data;
  data = readLevelData(level);
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