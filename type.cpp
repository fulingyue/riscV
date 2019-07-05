//
// Created by 傅凌玥 on 2019/7/2.
//
#include "type.h"

Type::Type(int inp){
  auto i = 0;
  opcode = inp & 127;
  inp >>= 7;
  while(inp > 0){
    bits[i] = inp & 1;
    inp >>= 1;
  }
}
TYPE Type::getType(){
  if(opcode == 51){
    return R-type;
  }
  else if(opcode == 3 || opcode == 103){
    return I-type;
  }
  else if(opcode == 19){
    if(bits[5] == 1 && bits[6] == 0 && bits[7] == 1){
      return R-type;
    }
    else if (bits[5] == 1 && bits[6] == 0 && bits[7] == 0){
      return R-type;
    }
    return I-type;
  }
  else if(opcode == 35){
    return S-type;
  }
  else if(opcode == 99){
    return B-type;
  }
  else if(opcode == 55 || opcode == 23){
    return U-type;
  }
  else if(opcode == 111){
    return J-type;
  }
}

virtual InstType R_Type::getInstType(){
  if(func7 == 0){
    if(func3 == 1)
      if(opnode == 19)
        return SLLI;
      else return SLL;
    if(func3 == 5)
      if(opnode == )
      return SRLI;
    if(func3 == 0)
      return ADD;
    if(func3 == 2)
      return SLT;
    if(func3 == 3)
      return SLTU;
    if(func3 == 4)
      return XOR;
  }
}