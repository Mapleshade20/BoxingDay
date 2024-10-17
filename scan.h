#include <iostream>
#include <string>
#include <sstream>

struct Instruction
{
    std::string name; // 指令类型
    int param;        // 操作位置
    bool is_box;      // 判断是否需要读取param
};

int strtoint(std::string a);
Instruction *scanins();