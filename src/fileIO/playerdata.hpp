#include <string>
#include <vector>

struct PlayerData {
  int level;
  int min_instructions;
  int min_steps;
};

class DataManager {
private:
  std::string home_dir;
  std::string filename;

public:
  DataManager();
  // int readPassedLevels();
  std::vector<PlayerData> readLevelData();
  void writeData(int level, int instructions, int steps);
};
