#ifndef imageloader_H
#define imageloader_H

#include "/images/logo.h"
#include "vex.h"
#include <list>
#include <regex>
#include <sstream>


char map[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int str_to_int(const std::string &str) {
  std::stringstream ss(str);
  int num;
  ss >> num;
  return num;
}

std::vector<std::string> resplit(const std::string &s,
                                 std::string rgx_str = "(?=[a-z^#...])") {
  std::vector<std::string> elems;

  std::regex rgx(rgx_str);

  std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
  std::sregex_token_iterator end;

  while (iter != end) {
    // std::cout << "S43:" << *iter << std::endl;
    elems.push_back(*iter);
    ++iter;
  }

  return elems;
}
void DisplayImg() {
  std::vector<std::string> instructions = resplit(rle);

  int n = sizeof(map) / sizeof(map[0]);

  int currentInstruction = 0;
  int i = 0;
  int maxI = 0;
  vex::color col = vex::color().web("#FFFFFF");
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 480; x++) {
      if (i >= maxI) {
        i = 0;

        std::string instruction = instructions[currentInstruction];

        while (instruction == "") {
          currentInstruction++;
          instruction = instructions[currentInstruction];
        }

        if (instruction[0] != '#') {
          auto itr = std::find(map, map + n, instruction[0]);
          int index = std::distance(map, itr);
          std::string hex = pointers[index];
          std::string regHex = "";
          regHex += '#';
          regHex += hex[1];
          regHex += hex[1];
          regHex += hex[2];
          regHex += hex[2];
          regHex += hex[3];
          regHex += hex[3];
          col = vex::color().web(regHex.c_str());
          Brain.Screen.setPenColor(col);
          maxI = str_to_int(instruction.substr(1, instruction.length() - 1));

        } else {
          std::string hex = instruction.substr(0, 4);
          std::string regHex = "";
          regHex += '#';
          regHex += hex[1];
          regHex += hex[1];
          regHex += hex[2];
          regHex += hex[2];
          regHex += hex[3];
          regHex += hex[3];
          col = vex::color().web(regHex.c_str());
          Brain.Screen.setPenColor(col);

          maxI = str_to_int(instruction.substr(4, instruction.length() - 4));
        }
        currentInstruction++;
      }
      Brain.Screen.drawPixel(x, y);
      i++;
    }
  }
}

#endif