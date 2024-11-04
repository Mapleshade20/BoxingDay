#include "scan.h"
using namespace std;

int strtoint(string a) {
  int len = a.length(), ans = 0;
  for (int i = 0; i < len; i++) {
    ans *= 10;
    if (int(a[i]) >= 48 && int(a[i]) <= 57) {
      ans += int(a[i]) - 48;
    } else {
      ans = -1;
      break;
    }
  }
  return ans;
}

Instruction *scanins() {
  Instruction *program = new Instruction[256];
  for (int i = 0; i < 256; i++) {
    program[i].name = "";
    program[i].param = 0;
    program[i].is_box = true;
  }
  int i = 0;
  while (i < 256) {
    string s_input;
    getline(cin, s_input); // 输入一行指令
    stringstream ss(s_input);
    string str;
    getline(ss, str, ' ');
    if (str == "end") {
      program[i].name = str;
      break;
    }
    program[i].name = str;     // 第一个词存入指令类型
    if (getline(ss, str, ' ')) // 第二个词存在
    {
      int paramm = strtoint(str);
      if (paramm == -1)
        program[i].name = "error"; // 第二个词不是合法数字
      else {
        program[i].param = paramm;
        program[i].is_box = false; // 有数字，不是box
      }
      if (getline(ss, str, ' ')) // 第三个词存在，语法错误
        program[i].name = "error";
    } else
      program[i].is_box = true;
    i++;
  }
  string valid_name[8] = {"inbox",  "outbox",   "add",  "sub",
                          "copyto", "copyfrom", "jump", "jumpifzero"};
  for (int j = 0; j < i; j++) {
    int k = 0;
    while (k < 8) {
      if (program[j].name == valid_name[k])
        break;
      k++;
    }
    if (k == 8 || (k <= 1 && program[j].is_box == false) ||
        (k > 1 &&
         program[j].is_box == true)) // 指令名称错误或box有数字或非box无数字
      program[j].name = "error";
  }
  return program;
}
