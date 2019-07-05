//
// Created by 傅凌玥 on 2019/7/2.
//

#ifndef RISC_V_TYPE_H
#define RISC_V_TYPE_H

enum TYPE{
  R_type,I_type,S_type,B_type,U_type,J_type,
};
enum InstType{
  SLLI,SRLI,SRAI,ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,
  JALR,LB,LH,LW,LBU,LHU,ADDI,SLTI,SLTIU,XORI,ORI,ANDI,
  SB,SH,SW,
  BEQ,BNE,BLT,BGE,BLTU,BGEU,
  LUI,AUIPC,
  JAL,
};

class Type{
  Type() = default;
  Type(int inp);
  TYPE getType();
  virtual InstType getInstType() = 0;
//  virtual execute() = 0;
  virtual int getImm() = 0;
private:
  int opcode;
   bits[25] = {0};
   imm[32] = {0};
};


Class R_Type:public Type{
  R_Type(){
    for(auto i = 0;i <= 4;++i){
      rd << 1;
      rd += bits[i];
    }
    for(auto i = 5;i <= 7;++i){
      func3 << 1;
      func3 += bits[i];
    }
    for(auto i = 8;i <= 12;++i){
      rs1 << 1;
      rs1 += bits[i];
    }
    for(auto i = 13;i <= 17;++i){
      rs2 << 1;
      rs2 += bits[i];
    }
    for(auto i = 18;i <= 24;++i){
      func7 << 1;
      func7 += bits[i];
    }
  }
  virtual InstType getInstType();
  virtual int getImm() {
    return 99990;
  };
private:
  int rs1 = 0,rs2 = 0,func7 = 0,func3 = 0,rd = 0;
};

Class I_Type:public Type{
  virtual int getImm();
};

Class S_Type:public Type{
  virtual int getImm();
};

Class B_Type:public Type{
  virtual int getImm();
};
Class U_Type:public Type{
  virtual int getImm();
};
Class J_Type:public Type{
  virtual int getImm();
};






#endif //RISC_V_TYPE_H
