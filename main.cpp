#include <iostream>
#include "Simulator.h"
#include <string>
#include <stdio.h>

int main() {

//  freopen("../../src/bulgarian.data", "r", stdin);
//  freopen("../foolout", "w", stdout);
  std::string inp;
  char ch;
  while (std::cin.get(ch)) {
    inp.push_back(ch);
  }
  Simulator sim(inp);
  auto res = sim.run();
  std::cout << std::dec << res << std::endl;
  return 0;
}