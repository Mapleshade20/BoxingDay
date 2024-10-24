#include <iostream>
using namespace std;
#include "scan.h"
int main() {
  Instruction *program = scanins();
  int i = 0;
  while (program[i].name != "end") {
    cout << program[i].name << ' ';
    if (program[i].is_box == false)
      cout << program[i].param << ' ';
    cout << endl;
    i++;
  }
  return 0;
}