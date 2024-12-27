#include "types.hpp"

std::string Register::getContent() const {
  return (this->is_empty ? "" : std::to_string(this->hand));
}

Register::Register() {
  this->current_tile = -1;
  this->hand = 0;
  this->is_empty = true;
}

std::string Tile::getContent() const {
  return (this->is_empty ? "" : std::to_string(this->value));
}

Tile::Tile() {
  this->value = 0;
  this->is_empty = true;
}

InstructionType Instruction::fromString(std::string &name) {
  if (name == "inbox")
    return InstructionType::INBOX;
  else if (name == "outbox")
    return InstructionType::OUTBOX;
  else if (name == "add")
    return InstructionType::ADD;
  else if (name == "sub")
    return InstructionType::SUB;
  else if (name == "copyto")
    return InstructionType::COPYTO;
  else if (name == "copyfrom")
    return InstructionType::COPYFROM;
  else if (name == "jump")
    return InstructionType::JUMP;
  else if (name == "jumpifzero")
    return InstructionType::JUMPIFZERO;
  else {
    return InstructionType::ERROR;
  }
}

void Program::setInstructions(std::vector<Instruction> instructions) {
  this->instructions = instructions;
}

void Program::addInstruction(const Instruction &inst) {
  instructions.push_back(inst);
}
void Program::clear() { instructions.clear(); }
const Instruction &Program::at(int index) const {
  return instructions.at(index);
}
int Program::size() const { return instructions.size(); }
