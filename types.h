// types.h - Basic type definitions

enum class InstructionType {
  INBOX,
  OUTBOX,
  ADD,
  SUB,
  COPYTO,
  COPYFROM,
  JUMP,
  JUMPIFZERO,
  END
};

enum class ExecutionError {
  NONE,
  INVALID_INSTRUCTION,
  EMPTY_HAND,
  INVALID_JUMP,
  OUT_OF_BOUNDS,
  EMPTY_INBOX // only happens in debug stage
};

struct Register {
  int current_tile;
  int dest_tile;
  int hand;
  bool is_empty;
};

struct Tile {
  int value;
  bool is_empty;
};
