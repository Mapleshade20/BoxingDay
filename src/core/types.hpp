#pragma once

#include <string>
#include <vector>

enum class InstructionType {
  INBOX,
  OUTBOX,
  ADD,
  SUB,
  COPYTO,
  COPYFROM,
  JUMP,
  JUMPIFZERO,
  END,
  ERROR
};

enum class ExecutionError {
  NONE,
  LEVEL_NOT_FOUND,
  INVALID_INSTRUCTION,
  EMPTY_HAND,
  EMPTY_TILE,
  INVALID_JUMP,
  OUT_OF_BOUNDS,
};

struct Register {
  int current_tile;
  int hand;
  bool is_empty;
};

struct Tile {
  int value;
  bool is_empty;
};

class Instruction {
public:
  InstructionType type;
  int param;

  static Instruction fromString(const std::string &name, int param);
};

class Program {
private:
  std::vector<Instruction> instructions;

public:
  void addInstruction(const Instruction &inst);
  void clear();
  const Instruction &at(int index) const;
  int size() const;
};
