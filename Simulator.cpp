//
// Created by 傅凌玥 on 2019/7/2.
//
#include "Simulator.h"
#include <sstream>
#include <string>
#include <iostream>
#include <bitset>
#include <cstring>
#include <cassert>
#include <array>
std::string str[] = {
    "SLLI", "SRLI", "SRAI", "ADD", "SUB", "SLL", "SLT",  "SLTU", "XOR",   "SRL",   "SRA",   "OR",   "AND",
    "JALR", "LB",   "LH",   "LW",  "LBU", "LHU", "ADDI", "SLTI", "SLTIU", "XORI",  "ORI",   "ANDI", "SB",
    "SH",   "SW",   "BEQ",  "BNE", "BLT", "BGE", "BLTU", "BGEU", "LUI",   "AUIPC", "JAL",
};

Simulator::Simulator(const std::string &inp) {
  int memoryPos;
  std::stringstream ss(inp);
//  // // std::cout << ss.str() << std::endl;
  while (!ss.eof()) {
    while (std::isspace(ss.peek()))
      ss.get();
    if (ss.peek() == '@') {
      ss.get();
      ss >> std::hex >> memoryPos;
      continue;
    }
    std::int32_t byte;
    ss >> std::hex >> byte;
    memory[memoryPos++] = (char)byte;
  }
}
namespace {
std::uint32_t getBits(std::uint32_t x, std::size_t l, std::size_t r) {
  return (x >> l) & ((1 << (r - l + 1)) - 1);
}
}
int Simulator::run() {
  int i = 0;
  while (true) {
    // std::cout << "===========" << "  " << std::dec << i<< std::hex <<  std::endl;
    reg[0] = 0;
//    std:: cout << "reg10:" << reg[10] << std::endl;
    If();

    if (buffer[0].inst == 0xc68223) {
      return ((unsigned int)reg[10]) % 256u;
    }
//    std:: cout << "reg10:" << reg[10] << std::endl;
    Id();
//    std:: cout << "reg10:" << reg[10] << std::endl;
    Exe();


    Mem();
    Wb();
    //for(auto i = 0;i < 32;++i){
      //if(reg[i]);
         // std::cout << std::dec << reg[i]  << std::hex << '\t';
    //}
    // std::cout << std::endl;
    ++i;
  }
}
void Simulator::If() {
  buffer[0].inst = 0;
  buffer[0].inst = *(std::uint32_t *)(memory + pc);
  assert(buffer[0].inst);
//  if( pc == 0x1120)
//    assert(false);
  pc += 4;
    // std::cout << std::hex <<  pc - 4 << std::endl;
   // std::cout << std::bitset<32>(buffer[0].inst) << std::endl;
//  for(auto i = 0;i < 32;++i){
//    // // std::cout << std::hex << reg[i] << std::endl;
//  }

}
void Simulator::Id() {
  int tmp;
  buffer[1] = buffer[0];
  buffer[1].imm = 0;
  int opcode = buffer[0].inst & 127;
  int func3, func7;
  switch (opcode) {
  case 51: // R
    func7 = getBits(buffer[0].inst,25,31);
    tmp = getBits(buffer[0].inst, 20,24);
    buffer[1].rs2 = reg[tmp];
    tmp = getBits(buffer[0].inst,15,19);
    buffer[1].rs1 = reg[tmp];
    func3 = getBits(buffer[0].inst,12,14);
    buffer[1].rd = getBits(buffer[0].inst,7,11);
    switch (func7) {
    case 0: // R
      switch (func3) {
      case 0b0000:
        buffer[1].type = ADD;
        break;
      case 1:
        buffer[1].type = SLL;
        break;
      case 2:
        buffer[1].type = SLT;
        break;
      case 3:
        buffer[1].type = SLTU;
        break;
      case 4:
        buffer[1].type = XOR;
        break;
      case 5:
        buffer[1].type = SRL;
        break;
      case 6:
        buffer[1].type = OR;
        break;
      default:
        buffer[1].type = AND;
      }
      break;
    default:
      switch (func3) {
      case 0:
        buffer[1].type = SUB;
        break;
      default:
        buffer[1].type = SRA;
      }
    }
    break;
  case 3://I
    func3 = getBits(buffer[0].inst,12,14);
    switch (func3) {
    case 0:
      buffer[1].type = LB;
      break;
    case 1:
      buffer[1].type = LH;
      break;
    case 2:
      buffer[1].type = LW;
      break;
    case 4:
      buffer[1].type = LBU;
      break;
    default:
      buffer[1].type = LHU;
    }
  case 103: // I
   if (opcode == 103)
    buffer[1].type = JALR;

    if (buffer[0].inst >> 31) {
      buffer[1].imm = (1 << 21) - 1;
      buffer[1].imm <<= 12;
    }
    buffer[1].imm += getBits(buffer[0].inst,20,31);
    tmp = getBits(buffer[0].inst, 15, 19);
    buffer[1].rs1 = reg[tmp];
    buffer[1].rd = getBits(buffer[0].inst, 7, 11);
//    // // std::cout << "immnow: " << std::hex <<  buffer[1].imm << std::endl;

    break;
  case 35: // S
    tmp = getBits(buffer[0].inst,20,24);
    buffer[1].rs2 = reg[tmp];
    tmp = getBits(buffer[0].inst,15,19);
    buffer[1].rs1 = reg[tmp];
    func3 = getBits(buffer[0].inst,12,14);
    if (buffer[0].inst >> 31)
      buffer[1].imm = (1 << 21) - 1;
    buffer[1].imm <<= 6;
    buffer[1].imm += getBits(buffer[0].inst, 25,30);
    buffer[1].imm <<= 5;
    buffer[1].imm += getBits(buffer[0].inst, 7, 11);
    switch (func3) {
    case 0:
      buffer[1].type = SB;
      break;
    case 1:
      buffer[1].type = SH;
      break;
    default:
      buffer[1].type = SW;
      break;
    }
    break;
  case 99: // B
    tmp = getBits(buffer[0].inst, 20, 24);
    buffer[1].rs2 = reg[tmp];
    tmp = getBits(buffer[0].inst, 15, 19);
    buffer[1].rs1 = reg[tmp];
    func3 = getBits(buffer[0].inst, 12, 14);
    if (buffer[0].inst >> 31) {
      buffer[1].imm = (1 << 20) - 1;
      buffer[1].imm <<= 1;
    }
    buffer[1].imm += getBits(buffer[0].inst, 7, 7);
    buffer[1].imm <<= 6;
    buffer[1].imm += getBits(buffer[0].inst,25,30);
    buffer[1].imm <<= 4;
    buffer[1].imm += getBits(buffer[0].inst,8,11);
    buffer[1].imm <<= 1;
    switch (func3) {
    case 0:
      buffer[1].type = BEQ;
      break;
    case 1:
      buffer[1].type = BNE;
      break;
    case 4:
      buffer[1].type = BLT;
      break;
    case 5:
      buffer[1].type = BGE;
      break;
    case 6:
      buffer[1].type = BLTU;
      break;
    default:
      buffer[1].type = BGEU;
    }
    break;
  case 55:
    buffer[1].type = LUI;
  case 23: // U
    if (opcode == 23)
      buffer[1].type = AUIPC;

    buffer[1].rd = getBits(buffer[0].inst,7,11);
    buffer[1].imm = getBits(buffer[0].inst,12,31);
    buffer[1].imm <<= 12;
    break;
  case 111: // J
    buffer[1].rd = (buffer[0].inst >> 7) & 31;
    if (buffer[0].inst >> 31) {
      buffer[1].imm = (1 << 13) - 1;
    }
    buffer[1].imm <<= 8;
    tmp = getBits(buffer[0].inst, 12, 19);
    buffer[1].imm += tmp;
    buffer[1].imm <<= 1;
    buffer[1].imm += getBits(buffer[0].inst, 20, 20);
    buffer[1].imm <<= 10;
    buffer[1].imm += getBits(buffer[0].inst, 21, 30);
    buffer[1].imm <<= 1;
    buffer[1].type = JAL;
//    // std::cerr << buffer[1].imm << std::endl;
    break;
  default:
    func3 = (buffer[0].inst >> 12) & 7;
    switch (func3) {
    case 0:
      buffer[1].type = ADDI;
    case 2:
      if (func3 == 2)
        buffer[1].type = SLTI;
    case 3:
      if (func3 == 3)
        buffer[1].type = SLTIU;
    case 4:
      if (func3 == 4)
        buffer[1].type = XORI;
    case 6:
      if (func3 == 6)
        buffer[1].type = ORI;
    case 7:
      if (func3 == 7)
        buffer[1].type = ANDI;
      tmp = getBits(buffer[0].inst, 15, 19);
//      // std::cerr << tmp << std::endl;
      buffer[1].rs1 = reg[tmp];

      buffer[1].rd = getBits(buffer[0].inst, 7, 11);

      if (buffer[0].inst >> 31) {
        buffer[1].imm = (1 << 20) - 1;
        buffer[1].imm <<= 12;
      }
      buffer[1].imm += (buffer[0].inst >> 20);
      break;
    case 1:
      buffer[1].type = SLLI;
    default: // R
      if (func3 == 5) {
        tmp = getBits(buffer[0].inst,30,30);
        if (!tmp)
          buffer[1].type = SRLI;
        else
          buffer[1].type = SRAI;
      }
      tmp = getBits(buffer[0].inst, 20, 24);
      buffer[1].rs2 = tmp;
      tmp = getBits(buffer[0].inst, 15, 19);
      buffer[1].rs1 = reg[tmp];
      buffer[1].rd = (buffer[0].inst >> 7) & 31;
    }
  }
   // std::cout << str[buffer[1].type] << std::endl;
   // std::cout << "imm:" << (int32_t)buffer[1].imm << '\t' <<
  //"rs1: " << (int32_t)buffer[1].rs1 << '\t' <<
  //"rs2: " << (int32_t)buffer[1].rs2 << '\t' <<
  //"rd :" << buffer[1].rd << std::endl;
}
void Simulator::Exe() {
  buffer[2] = buffer[1];
  switch (buffer[1].type) {

  case ADDI: // imm+rs1->rd
    buffer[2].exeRes = buffer[1].imm + buffer[1].rs1;
    break;
  case SLTI:
    buffer[2].exeRes = (int32_t)buffer[1].rs1 < (int32_t)buffer[1].imm;
    break;
  case SLTIU:
    buffer[2].exeRes = (uint32_t)buffer[1].rs1 < (uint32_t)buffer[1].imm;
    break;
  case XORI:
    buffer[2].exeRes = buffer[1].rs1 ^ buffer[1].imm;
    break;
  case ORI:
    buffer[2].exeRes = buffer[1].rs1 | buffer[1].imm;
    break;
  case ANDI:
    buffer[2].exeRes = buffer[1].rs1 & buffer[1].imm;
    break;
  case SLLI:
    buffer[2].exeRes = buffer[1].rs1 << ((uint32_t)buffer[1].rs2 & 31u);
//    // std::cerr << std::hex << buffer[1].rs1 << ' ' << buffer[1].rs2 << std::endl;
//    // std::cerr << std::hex <<  buffer[2].exeRes << std::endl;
    break;
  case SRLI:
    buffer[2].exeRes = (uint32_t)buffer[1].rs1 >> ((uint32_t)buffer[1].rs2 & 31u);
    break;
  case SRAI:

    buffer[2].exeRes = (buffer[1].rs1) >> ((uint32_t)buffer[1].rs2 & 31u);
//    // std::cout << "****" << ((uint32_t)buffer[1].rs2 & 31u) << std::endl;
    break;
  case ADD:
    buffer[2].exeRes = buffer[1].rs1 + buffer[1].rs2;
    break;
  case SUB:
    buffer[2].exeRes = buffer[1].rs1 - buffer[1].rs2;
    break;
  case SLT:
    buffer[2].exeRes = buffer[1].rs1 < buffer[1].rs2;
    break;
  case SLTU:
    buffer[2].exeRes = (uint32_t)buffer[1].rs1 < (uint32_t)buffer[1].rs2;
    break;
  case SLL:
    buffer[2].exeRes = buffer[1].rs1 << (buffer[1].rs2 & 31);
    break;
  case SRL:
    buffer[2].exeRes = (uint32_t)buffer[1].rs1 >> (uint32_t)(buffer[1].rs2 & 31);
    break;
  case SRA:
    buffer[2].exeRes = (buffer[1].rs1) >> (buffer[1].rs2 & 31);
    break;
  case XOR:
    buffer[2].exeRes = buffer[1].rs1 ^ buffer[1].rs2;
    break;
  case OR:
    buffer[2].exeRes = buffer[1].rs1 | buffer[1].rs2;
    break;
  case AND:
    buffer[2].exeRes = buffer[1].rs1 & buffer[1].rs2;
    break;
  case JAL:
    buffer[2].exeRes = pc;
    pc += buffer[1].imm - 4;
    break;
  case JALR:
    buffer[2].exeRes = pc;
    pc = (buffer[1].imm + buffer[1].rs1) & (-2);
    break;
  case LUI:
    buffer[2].exeRes = buffer[1].imm;
    // std::cerr << "res: " << buffer[2].exeRes << std:: endl;
    break;
  case LB:
    buffer[2].exeRes =  buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes && buffer[2].exeRes < 0x20000);
    break;
  case LH:
    buffer[2].exeRes = buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes && buffer[2].exeRes < 0x20000);
    break;
  case LW:
   buffer[2].exeRes = (std::int32_t)buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes && buffer[2].exeRes < 0x20000);
    break;
  case LBU:
    buffer[2].exeRes = (std::int32_t)buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes &&  buffer[2].exeRes < 0x20000);
    break;
  case LHU:
    buffer[2].exeRes = (std::int32_t)buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes && buffer[2].exeRes < 0x20000);
    break;
  case SB:
    buffer[2].exeRes = (std::int32_t)buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes && buffer[2].exeRes < 0x20000);
    break;
  case SH:
    buffer[2].exeRes =  (std::int32_t)buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes && buffer[2].exeRes < 0x20000);
    break;
  case SW:
    buffer[2].exeRes = (std::int32_t)buffer[1].rs1 + (std::int32_t)buffer[1].imm;
    assert(buffer[2].exeRes && buffer[2].exeRes < 0x20000);
    break;
  case BEQ:
    if(buffer[1].rs1 == buffer[1].rs2)
      pc += buffer[1].imm - 4;
    break;
  case BNE:
    if(buffer[1].rs1 != buffer[1].rs2)
      pc += buffer[1].imm - 4;
    break;
  case BLT:
    if(buffer[1].rs1 < buffer[1].rs2)
      pc += buffer[1].imm - 4;
    break;
  case BLTU:
    if((uint32_t)buffer[1].rs1 < (uint32_t)buffer[1].rs2)
      pc += buffer[1].imm - 4;
    break;
  case BGE:
    if(buffer[1].rs1 >= buffer[1].rs2)
      pc += buffer[1].imm - 4;
    break;
  case BGEU:
    if((uint32_t)buffer[1].rs1 >= (uint32_t)buffer[1].rs2)
      pc += buffer[1].imm - 4;
    break;

  case AUIPC:
    buffer[2].exeRes = pc + buffer[1].imm - 4;
  default:
    break;

  }
//  // // std::cout << "imm2:" << buffer[2].imm << std::endl;
}
void Simulator::Mem() {
  buffer[3] = buffer[2];
  switch (buffer[2].type){
  case LB://TODO
    buffer[3].exeRes = *(int8_t *)(memory + buffer[2].exeRes);
    break;
  case LH:
    buffer[3].exeRes = *(int16_t *)(memory + buffer[2].exeRes);
    break;
  case LW:
    buffer[3].exeRes = *(int32_t *)(memory + buffer[2].exeRes);
    break;
  case LBU:
    buffer[3].exeRes = *(uint8_t *)(memory + buffer[2].exeRes);
    break;
  case LHU:
    buffer[3].exeRes = *(uint16_t *)(memory + buffer[2].exeRes);
    break;
  case SB:{
    char tmp = buffer[2].rs2;
    memcpy(memory + buffer[2].exeRes, &tmp, 1);
    break;
  }
  case SH:{
    short tmp = buffer[2].rs2;
    memcpy(memory + buffer[2].exeRes, &tmp, 2);
    break;
  }
  case SW:{
    memcpy(memory + buffer[2].exeRes, &buffer[2].rs2, 4);
    break;
  }
  default:
    break;
  }
}
void Simulator::Wb() {
  switch (buffer[3].type){
  case BEQ:
  case BNE:
  case BLT:
  case BGE:
  case BLTU:
  case BGEU:
  case SB:
  case SH:
  case SW:
    break;
  default:
    reg[buffer[3].rd] = buffer[3].exeRes;
  }
}
