#include <cstdlib>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "libtui.hpp"

// Function to clear the console
void clearConsole() {
  system("clear"); // Use "cls" for Windows
}

// Function to set terminal to non-blocking mode
void setNonBlockingInput() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);          // Get current terminal settings
  newt = oldt;                             // Copy settings
  newt.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Set new settings
}

// Function to reset terminal settings
void resetTerminal() {
  struct termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  oldt.c_lflag |= (ICANON | ECHO); // Enable canonical mode and echo back
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Reset terminal settings
}

// Function to check if a key has been pressed
bool kbhit() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  int bytesWaiting;
  ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings

  return bytesWaiting > 0; // Return true if there are bytes waiting to read
}
