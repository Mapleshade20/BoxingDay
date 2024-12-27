#include <unistd.h>

#include <iostream>

#include "../src/ui/libtui.hpp"

using namespace std;

// Game constants
const int WIDTH = 21;
const int HEIGHT = 10;

// Game elements
char level[HEIGHT][WIDTH] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
     '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ',
     '#', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
     ' ', ' ', '#', '#', '#', '#', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#',
     '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
     ' ', '#', ' ', ' ', ' ', ' ', ' ', 'E', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
     '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
};

// Screen buffer to track changes
char screenBuffer[HEIGHT][WIDTH];

// Player position
int playerX = 1;
int playerY = 1;

// Function to initialize screen buffer
void initScreenBuffer() {
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      screenBuffer[y][x] = level[y][x];
    }
  }
}

// Function to update a single position on screen
void updatePosition(int x, int y, char newChar) {
  if (screenBuffer[y][x] != newChar) {
    moveCursor(x + 1, y + 1);
    cout << newChar << flush;
    screenBuffer[y][x] = newChar;
  }
}

// Function to draw the level initially
void drawInitialLevel() {
  clearConsole();
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      char displayChar = (x == playerX && y == playerY) ? 'P' : level[y][x];
      cout << displayChar;
      screenBuffer[y][x] = displayChar;
    }
    cout << endl;
  }
}

// Function to update player position
void updatePlayerPosition(int oldX, int oldY, int newX, int newY) {
  // Restore old position
  updatePosition(oldX, oldY, level[oldY][oldX]);

  // Draw new position
  updatePosition(newX, newY, 'P');
}

// Function to check for collision with walls or enemies
bool isCollision(int newX, int newY) {
  return (level[newY][newX] == '#' || level[newY][newX] == 'E');
}

// Main game loop
void gameLoop() {
  drawInitialLevel();

  while (true) {
    if (kbhit()) {
      char input;
      cin >> input;

      int newX = playerX;
      int newY = playerY;

      switch (input) {
        case 'w':
          newY--;
          break;
        case 's':
          newY++;
          break;
        case 'a':
          newX--;
          break;
        case 'd':
          newX++;
          break;
        case 'q':
          showCursor();
          return;
      }

      if (!isCollision(newX, newY)) {
        int oldX = playerX;
        int oldY = playerY;
        playerX = newX;
        playerY = newY;
        updatePlayerPosition(oldX, oldY, newX, newY);
      }
    }
    usleep(1000000 / 60);  // 60 FPS
  }
}

int main() {
  setNonBlockingInput();
  hideCursor();

  cout << "Welcome to the Text-Based Game!" << endl;
  cout << "Controls: w (up), s (down), a (left), d (right), q (quit)" << endl;
  usleep(3000000);

  initScreenBuffer();
  gameLoop();

  showCursor();
  resetTerminal();
  return 0;
}
