#include "ui/ui.hpp"
#include <iostream>

int main() {
  try {

    GameUI ui;

    // ui.home();
    ui.run();

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
