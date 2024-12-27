#include <chrono>
#include <iostream>
#include <thread>

#include "../src/ui/libtui.hpp"

using namespace std;

// Game constants
const int WIDTH = 21;
const int HEIGHT = 10;

// Game elements
char level[HEIGHT][WIDTH] = {
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
		 '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'E',
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

// Player position
int playerX = 1;
int playerY = 1;

// Function to draw the level
void drawLevel() {
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (x == playerX && y == playerY) {
				cout << "P";	// Draw player
			} else {
				cout << level[y][x];	// Draw level elements
			}
		}
		cout << endl;
	}
}

// Function to check for collision with walls or enemies
bool isCollision(int newX, int newY) {
	if (level[newY][newX] == '#' || level[newY][newX] == 'E') {
		return true;	// Collision detected
	}
	return false;	 // No collision
}

// Main game loop
void gameLoop() {
	while (true) {
		clearConsole();
		drawLevel();

		// Check for user input
		if (kbhit()) {	// Check if a key is pressed
			char input;
			cin >> input;	 // Read the key pressed

			int newX = playerX;
			int newY = playerY;

			switch (input) {
				case 'w':
					newY--;
					break;	// Move up
				case 's':
					newY++;
					break;	// Move down
				case 'a':
					newX--;
					break;	// Move left
				case 'd':
					newX++;
					break;	// Move right
				case 'q':
					return;	 // Quit the game
			}

			// Check for collisions before moving the player
			if (!isCollision(newX, newY)) {
				playerX = newX;
				playerY = newY;
			}
		}

		this_thread::sleep_for(chrono::milliseconds(100));	// Control frame rate
	}
}

int main() {
	setNonBlockingInput();	// Set terminal to non-blocking mode

	cout << "Welcome to the Text-Based Game!" << endl;
	cout << "Controls: w (up), s (down), a (left), d (right), q (quit)" << endl;
	this_thread::sleep_for(chrono::milliseconds(3000));	 // Control frame rate

	gameLoop();	 // Start the game loop

	resetTerminal();	// Reset terminal settings before exiting

	return 0;
}
