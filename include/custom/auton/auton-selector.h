#ifndef autonselector_h
#define autonselector_h

#include "/custom/button.h"
#include "/custom/util.h"
#include "vex.h"

util::side curSide = none;

Button autonButtons[] = {
    Button(10, 10, 200, 50, "Left", vex::green, vex::black),
    Button(220, 10, 200, 50, "Right", vex::white, vex::black),
};

void ButtonLoop(competition Competition) {
  Brain.Screen.clearScreen(vex::white);
  if (!Competition.isEnabled()) {
    for (int i = 0; i < 2; i++) {
      autonButtons[i].render();
      if (autonButtons[i].isClicked()) {
        autonButtons[i].buttonColor = vex::white;
        autonButtons[i].buttonColor = vex::green;
        curSide = i == 0 ? util::left : util::right;
      }
    }
  }
  Brain.Screen.render();
}

#endif