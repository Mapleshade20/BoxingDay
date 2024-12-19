#pragma once

#include "../core/gamestate.hpp"
#include "renderer.hpp"

class CanvasRenderer : public Renderer {
public:
  CanvasRenderer(const LevelData &l);
  void renderLine(int x, int y, int length, bool is_horizontal);
  void setup();
};

enum class RenderMode { TEXT, ALL, CLEAR_AND_ALL };

class MovableBoxRenderer : public Renderer {
protected:
  std::vector<Coordinate> showup_coords;
  static const int BORDER_WIDTH = 1;
  static const int BORDER_HEIGHT = 1;
  static const int BLANK_WIDTH = 5;
  Coordinate prev_coord;

public:
  MovableBoxRenderer(int start_x, int start_y);

  // Change base coordinate
  void moveTo(Coordinate dest);

  // Update render in-game
  void update(std::string word, RenderMode mode, bool is_highlighted);
};

class RegRenderer : public MovableBoxRenderer {
protected:
  Register prev_reg;
  static const int N_PER_ROW = 5;
  static const int X_DELTA = 8;
  static const int Y_DELTA = 16;
  int sleep_ns;

public:
  RegRenderer(int start_x, int start_y, const GameState &state,
              float speed_rate);

  // Initial render
  void setup(const GameState &state);

  // Update render in-game
  void refresh(const GameState &state);
};

class TilesRenderer : public MovableBoxRenderer {
protected:
  int prev_tile_idx;
  static const int N_PER_ROW = 5;
  static const int X_DELTA = 8;
  static const int Y_DELTA = 6;

public:
  TilesRenderer(int start_x, int start_y, const GameState &state);

  // Initial render
  void setup(const GameState &state);

  // Update render in-game
  void refresh(const GameState &state);
};

class SequenceRenderer : public MovableBoxRenderer {
private:
  // bool(state.reg.current_tile + 2) == 1 if inbox
  static const int N_PER_COLUMN = 6;
  static const int X_DELTA = 64;
  static const int Y_DELTA = 3;

public:
  SequenceRenderer(int start_x, int start_y);
  void renderVisibleInbox(const GameState &state);
  void renderVisibleOutbox(const GameState &state);

  // Initial render
  void setup(const GameState &state);

  // Update render in-game
  void refresh(const GameState &state);
};
