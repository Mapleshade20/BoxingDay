#include "libtui.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

// Clear console screen
void clearConsole() { system("clear"); }

// Set up terminal for game
void setNonBlockingInput() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);           // Get current terminal settings
  newt = oldt;                              // Copy settings
  newt.c_lflag &= ~(ICANON | ECHO);         // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Set new settings
}

// Restore terminal settings
void resetTerminal() {
  struct termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  oldt.c_lflag |= (ICANON | ECHO);  // Enable canonical mode and echo back
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Reset terminal settings
}

// Returns true if a key is being pressed
bool kbhit() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  int bytesWaiting;
  ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore old settings

  return bytesWaiting > 0;  // Return true if there are bytes waiting to read
}

// Move cursor to specific position (1-based!)
void moveCursor(int x, int y) {
  std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << std::flush;
}

void hideCursor() { std::cout << "\033[?25l" << std::flush; }

void showCursor() { std::cout << "\033[?25h" << std::flush; }
