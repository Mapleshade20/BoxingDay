#include "types.hpp"

Instruction Instruction::fromString(const std::string &name, int param) {
  Instruction ans;
  if (name == "error") {
    ans.type = InstructionType::ERROR;
    ans.param = -2;
    return ans;
  }
  ans.param = param;
  if (name == "inbox" && param == -1)
    ans.type = InstructionType::INBOX;
  else if (name == "outbox" && param == -1)
    ans.type = InstructionType::OUTBOX;
  else if (name == "add" && param != -1)
    ans.type = InstructionType::ADD;
  else if (name == "sub" && param != -1)
    ans.type = InstructionType::SUB;
  else if (name == "copyto" && param != -1)
    ans.type = InstructionType::COPYTO;
  else if (name == "copyfrom" && param != -1)
    ans.type = InstructionType::COPYFROM;
  else if (name == "jump" && param != -1)
    ans.type = InstructionType::JUMP;
  else if (name == "jumpifzero" && param != -1)
    ans.type = InstructionType::JUMPIFZERO;
  else {
    ans.type = InstructionType::ERROR;
    ans.param = -2;
  }
  return ans;
}

void Program::addInstruction(const Instruction &inst) {
  instructions.push_back(inst);
}
void Program::clear() { instructions.clear(); }
const Instruction &Program::at(int index) const {
  return instructions.at(index);
}
const int Program::size() const { return instructions.size(); }
