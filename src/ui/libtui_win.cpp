#include <conio.h>		// For kbhit() and getch()
#include <windows.h>	// For Windows-specific functions

#include <cstdlib>	// For system()

// Function to clear the console
void clearConsole() {
	system("cls");	// Clear console on Windows
}

// Function to set terminal to non-blocking mode
void setNonBlockingInput() {
	// No direct equivalent in Windows; using kbhit() for non-blocking input
}

// Function to reset terminal settings
void resetTerminal() {
	// Not needed for basic console applications in Windows
}

// Function to check if a key has been pressed
bool kbhit() {
	return _kbhit();	// Use _kbhit() from conio.h for Windows
}
