#pragma once

void clearConsole();

void setNonBlockingInput();

void resetTerminal();

bool kbhit();

void moveCursor(int x, int y);

void hideCursor();

void showCursor();
