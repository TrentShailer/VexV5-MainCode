#ifndef EasyBinds_H
#define EasyBinds_H

#include "custom/better-motor.h"
#include "vex.h"
#include <iostream>
#include <math.h>
#include <string.h>

// ---
// Easily bind a motor to a button
// ---

using namespace vex;

int ResetToggle(void *arg) {
  bool canChange = (bool *)arg;
  canChange = false;
  this_thread::sleep_for(500);
  canChange = true;
  return 1;
}

class binding {
private:
  enum direction { forward, reverse, none };

public:
  controller::button buttonUp;
  controller::button buttonDn;
  BetterMotor mtr;
  bool toggle;
  int speed;

  bool canChange = true;
  bool isToggled = false;
  direction dir = none;
  int toggleSpeed = 0;

  binding(controller::button _buttonUp, controller::button _buttonDn,
          BetterMotor _mtr, int _speed, bool _toggle = false)
      : mtr(_mtr) {
    buttonUp = _buttonUp;
    buttonDn = _buttonDn;
    toggle = _toggle;
    speed = _speed;
  }

  void RunCtrl() {
    if (toggle) {
      if (buttonUp.pressing() && canChange) {
        if (dir == direction::forward) {
          isToggled = false;
          dir = direction::none;
          toggleSpeed = 0;
        } else {
          isToggled = true;
          dir = direction::forward;
          toggleSpeed = speed;
        }
      } else if (buttonDn.pressing() && canChange) {
        if (dir == direction::reverse) {
          isToggled = false;
          dir = direction::none;
          toggleSpeed = 0;
        } else {
          isToggled = true;
          dir = direction::reverse;
          toggleSpeed = -speed;
          void *arg = &canChange;
          vex::thread reset(ResetToggle, arg);
        }
      }

      mtr.Drive(toggleSpeed);

    } else {
      if (buttonUp.pressing()) {
        mtr.Drive(speed);
      } else if (buttonDn.pressing()) {
        mtr.Drive(-speed);
      } else {
        mtr.Drive(0);
      }
    }
  }
};

#endif