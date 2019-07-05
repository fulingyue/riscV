//
// Created by 傅凌玥 on 2019/7/2.
//

#ifndef RISC_V_SIMULATOR_H
#define RISC_V_SIMULATOR_H

#include <string>

enum InstType{
  SLLI,SRLI,SRAI,ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,
  JALR,LB,LH,LW,LBU,LHU,ADDI,SLTI,SLTIU,XORI,ORI,ANDI,
  SB,SH,SW,
  BEQ,BNE,BLT,BGE,BLTU,BGEU,
  LUI,AUIPC,
  JAL,
};


class Simulator {

public :
  Simulator(const std::string &inp);
  int run();
private:
  void If();
  void Id();
  void Exe();
  void Mem();
  void Wb();

private:
  int8_t memory[0x20000];
  int32_t reg[32] = {0};
  struct Buffer {
    uint32_t inst;
    int32_t imm;
    int32_t rs1 = 0,rs2 = 0,rd = 0;//rd is index,rs1&rs2 are values
    int32_t exeRes;
    InstType type;
    Buffer() = default;
    Buffer(Buffer &o) = default;
  } buffer[4];
  int32_t pc = 0;

};
#endif //RISC_V_SIMULATOR_H
