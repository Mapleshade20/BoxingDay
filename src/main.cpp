#include <iostream>

#include "ui/libtui.hpp"
#include "ui/ui.hpp"

int main() {
  try {
    GameUI ui;

    // ui.home();
    ui.run();

  } catch (const std::exception &e) {
    std::cout << "Error: " << e.what() << std::endl;
    showCursor();
    resetTerminal();
    return 1;
  }

  return 0;
}
