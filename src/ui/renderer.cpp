#include "renderer.hpp"

#include <iostream>

#include "libtui.hpp"

Renderer::Renderer(int start_x, int start_y) {
  coord.x = start_x;
  coord.y = start_y;
}

std::string Renderer::space(int length) {
  std::string result = "";
  for (int i = 0; i < length; i++) {
    result += " ";
  }
  return result;
}

void Renderer::setShape(std::vector<std::string> s) { shape = s; }

void Renderer::renderShape(int dx, int dy) {
  for (int i = 0; i < shape.size(); i++) {
    moveCursor(coord.x + dx, coord.y + i + dy);
    std::cout << shape[i] << std::flush;
  }
}

void Renderer::renderWord(int dx, int dy, std::string word) {
  moveCursor(coord.x + dx, coord.y + dy);
  std::cout << word << std::flush;
}

void Renderer::renderShape(int dx, int dy, std::string color) {
  for (int i = 0; i < shape.size(); i++) {
    moveCursor(coord.x + dx, coord.y + i + dy);
    std::cout << color << shape[i] << RESET << std::flush;
  }
}

void Renderer::renderWord(int dx, int dy, std::string color, std::string word) {
  moveCursor(coord.x + dx, coord.y + dy);
  std::cout << color << word << RESET << std::flush;
}

void Renderer::clearShape(int dx, int dy) {
  for (int i = 0; i < shape.size(); i++) {
    moveCursor(coord.x + dx, coord.y + i + dy);
    std::cout << space(shape[i].length());
  }
  std::cout << std::flush;
}

void Renderer::clearShapeAbsolute(int x, int y) {
  for (int i = 0; i < shape.size(); i++) {
    moveCursor(x, y + i);
    std::cout << space(shape[i].length());
  }
  std::cout << std::flush;
}

void Renderer::clearArea(int dx, int dy, int width, int height) {
  for (int i = 0; i < height; i++) {
    moveCursor(coord.x + dx, coord.y + dy + i);
    std::cout << space(width);
  }
  std::cout << std::flush;
}
