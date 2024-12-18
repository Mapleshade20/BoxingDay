#pragma once

#include <string>
#include <vector>

struct Coordinate {
  int x;
  int y;
};

// A generic renderer that can render shapes and words
class Renderer {
protected:
  Coordinate coord;
  std::vector<std::string> shape;

public:
  // Set start position for rendering (1-based!)
  Renderer(int start_x, int start_y);

  // Return a string of spaces with the given length
  static std::string space(int length);

  // Store the shape to be rendered
  void setShape(std::vector<std::string> s);

  // Render shape at a relative position
  void renderShape(int dx, int dy);

  // Render word at a relative position
  void renderWord(int dx, int dy, std::string word);

  // Render shape in color at a relative position
  void renderShape(int dx, int dy, std::string color);

  // Render word in color at a relative position
  void renderWord(int dx, int dy, std::string color, std::string word);

  // Fill the shape area with spaces at a relative position
  void clearShape(int dx, int dy);

  // Fill the shape area with spaces at an absolute position
  void clearShapeAbsolute(int x, int y);

  // Fill a box area with spaces at a relative position
  void clearArea(int dx, int dy, int width, int height);
};
