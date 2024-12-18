#include "instances.hpp"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "libtui.hpp"

// render sequence (with delay):
// 1. move reg
// 2. update reg & tiles(if at mid) & inbox buffer(if at 0)
//    & outbox buffer(if at n)

MovableBoxRenderer::MovableBoxRenderer(int start_x, int start_y)
    : Renderer(start_x, start_y) {
  prev_coord = {start_x, start_y};
}

void MovableBoxRenderer::moveTo(Coordinate dest) { this->coord = dest; }

void MovableBoxRenderer::update(std::string word, RenderMode mode,
                                bool is_highlighted) {
  switch (mode) {
    case RenderMode::TEXT:
      clearArea(BORDER_WIDTH, BORDER_HEIGHT, BLANK_WIDTH, 1);
      break;
    case RenderMode::ALL:
      if (is_highlighted) {
        renderShape(0, 0, YELLOW);
      } else {
        renderShape(0, 0);
      }
      break;
    case RenderMode::CLEAR_AND_ALL:
      clearShapeAbsolute(prev_coord.x, prev_coord.y);
      if (is_highlighted) {
        renderShape(0, 0, YELLOW);
      } else {
        renderShape(0, 0);
      }
      break;
  }

  if (is_highlighted) {
    renderWord(BORDER_WIDTH + (BLANK_WIDTH - word.length()) / 2, BORDER_HEIGHT,
               YELLOW, word);
  } else {
    renderWord(BORDER_WIDTH + (BLANK_WIDTH - word.length()) / 2, BORDER_HEIGHT,
               word);
  }

  prev_coord = {coord.x, coord.y};
}

// TODO: check tiles no more than 10 in fileio parsing

RegRenderer::RegRenderer(int start_x, int start_y, const GameState &state,
                         float speed_rate)
    : MovableBoxRenderer(start_x, start_y) {
  this->shape = {
      " ----- ",   // line 0
      "|     |",   // line 1
      " ----- ",   // line 2
      "  -|-  ",   // line 3
      "  / \\  ",  // line 4
  };
  this->sleep_ns = int(900000.0 / speed_rate);
  this->prev_reg = Register();
  this->showup_coords = std::vector<Coordinate>(
      state.available_tiles + 2);  // for inbox and outbox positions
  for (int i = 0; i < state.available_tiles; i++) {
    this->showup_coords[i] = {start_x + (i % N_PER_ROW + 1) * X_DELTA,
                              start_y + (i / N_PER_ROW) * Y_DELTA};
  }
  this->showup_coords[state.available_tiles] = {start_x, start_y};
  this->showup_coords[state.available_tiles + 1] = {start_x + 6 * X_DELTA,
                                                    start_y};
}

void RegRenderer::setup(const GameState &state) {
  update(state.reg.getContent(), RenderMode::ALL, true);
  prev_reg = state.reg;
}

void RegRenderer::refresh(const GameState &state) {
  if (state.reg.current_tile != prev_reg.current_tile) {
    int target_idx = state.reg.current_tile < 0
                         ? state.available_tiles - 1 - state.reg.current_tile
                         : state.reg.current_tile;
    moveTo(this->showup_coords.at(target_idx));
    update(this->prev_reg.getContent(), RenderMode::CLEAR_AND_ALL, false);
    usleep(this->sleep_ns);
  }
  update(state.reg.getContent(), RenderMode::ALL, false);
  prev_reg = state.reg;
}

TilesRenderer::TilesRenderer(int start_x, int start_y, const GameState &state)
    : MovableBoxRenderer(start_x, start_y) {
  this->shape = {
      "/-----\\",  // line 0
      "|     |",   // line 1
      "\\-----/",  // line 2
  };
  this->showup_coords = std::vector<Coordinate>(state.available_tiles + 1);
  for (int i = 0; i < state.available_tiles; i++) {
    this->showup_coords[i] = {start_x + (i % N_PER_ROW) * X_DELTA,
                              start_y + (i / N_PER_ROW) * Y_DELTA};
  }
}

void TilesRenderer::setup(const GameState &state) {
  for (int i = 0; i < state.available_tiles; i++) {
    moveTo(this->showup_coords.at(i));
    update(state.tiles.at(i).getContent(), RenderMode::ALL, false);
    // TODO: show a #1 like tag
  }
  this->prev_tile_idx = -1;
}

void TilesRenderer::refresh(const GameState &state) {
  if (this->prev_tile_idx == state.reg.current_tile &&
      state.reg.current_tile >= 0) {
    update(state.tiles.at(state.reg.current_tile).getContent(),
           RenderMode::TEXT, true);
    return;
  }
  if (this->prev_tile_idx >= 0) {
    update(state.tiles.at(prev_tile_idx).getContent(),
           RenderMode::CLEAR_AND_ALL, false);
  }
  if (state.reg.current_tile >= 0) {
    moveTo(this->showup_coords.at(state.reg.current_tile));
    update(state.tiles.at(state.reg.current_tile).getContent(), RenderMode::ALL,
           true);
  }
  this->prev_tile_idx = state.reg.current_tile;
}

void CanvasRenderer::renderLine(int x, int y, int length, bool is_horizontal) {
  if (is_horizontal) {
    moveCursor(x, y);
    for (int i = 0; i < length; i++) {
      std::cout << "-";
    }
    std::cout << std::flush;
  } else {
    for (int i = 0; i < length; i++) {
      moveCursor(x, y + i);
      std::cout << "|";
    }
    std::cout << std::flush;
  }
}

void CanvasRenderer::start() {
  clearConsole();
  renderLine(1, 1, 83, true);
  renderLine(1, 1, 30, false);
  renderLine(1, 30, 83, true);
  renderLine(83, 1, 30, false);
  renderShape(2, 1);
}

CanvasRenderer::CanvasRenderer(const LevelData &l) : Renderer(1, 1) {
  this->shape = {"Level " + std::to_string(l.level) + ": " + l.title, ""};

  std::istringstream iss(l.description);
  std::string word;
  std::string line;
  const size_t maxLength = 70;
  while (iss >> word) {
    if (line.length() + word.length() + 1 > maxLength) {
      this->shape.push_back(line);
      line.clear();
    }
    if (!line.empty()) {
      line += " ";
    }
    line += word;
  }
  if (!line.empty()) {
    this->shape.push_back(line);
  }
}
