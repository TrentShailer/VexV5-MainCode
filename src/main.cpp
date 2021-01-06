#include "custom/auton/auton.h"
#include "custom/better-drivetrain.h"
#include "custom/better-motor.h"
#include "custom/easy-binds.h"
#include "custom/image/image-loader.h"
#include "vex.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vex_task.h>

using namespace vex;

competition Competition;

BetterDrivetrain dt = BetterDrivetrain();

#pragma region Vars

BetterMotor bExample = BetterMotor(example);

Vector2 maxDTSpeed = Vector2(40, 40);

#pragma endregion

util::side curSide = none;

bool canToggle = true;

void AutonSelection() {
  for (int i = 0; i < 20000 / 20; i++) {
    if (Controller1.ButtonLeft.pressing())
      curSide = util::left;
    else if (Controller1.ButtonRight.pressing())
      curSide = util::right;

    if (Controller1.ButtonUp.pressing())
      break;

    Controller1.Screen.clearScreen();
    Controller1.Screen.newLine();
    std::ostringstream s;
    s << "Current Auton: " << curSide;

    Controller1.Screen.print(s.str().c_str());

    wait(20, msec);
  }

  Controller1.Screen.clearScreen();
  Controller1.Screen.newLine();
  std::ostringstream s;
  s << "Confirmed Auton: " << curSide;

  Controller1.Screen.print(s.str().c_str());
}

void pre_auton(void) {
  vexcodeInit();

#pragma region MotorInit

  bExample = BetterMotor(bExample);

  dt.maxSpeed = Vector2(66, 66);
  dt.modSpeed = Vector2(10, 10);

  bExample.maxSpeed = 75;

#pragma endregion

  AutonSelection();
}

#pragma region Auton

void LeftAuton() {
}

void RightAuton() {
}

#pragma endregion

void autonomous(void) {
  Initialise(curSide);

  if (curSide == util::left)
    LeftAuton();
  else if (curSide == util::right)
    RightAuton();
}

int DisplayLoop() {
  while (true) {

    Controller1.Screen.clearScreen();
    Controller1.Screen.newLine();
    std::ostringstream s;
    s << "Max Speed: " << maxDTSpeed.x;

    Controller1.Screen.print(s.str().c_str());

    this_thread::sleep_for(20);
  }
  return 0;
}

void ControlDriveSpeed() {
  if (Controller1.ButtonUp.pressing() && maxDTSpeed.x + 10 < 105 && canToggle) {
    void *arg = &canToggle;
    vex::thread reset(ResetToggle, arg);

    maxDTSpeed.x += 10;
    maxDTSpeed.y += 10;
    dt.maxSpeed = maxDTSpeed;

  } else if (Controller1.ButtonDown.pressing() && maxDTSpeed.x - 10 > 5 &&
             canToggle) {
    void *arg = &canToggle;
    vex::thread reset(ResetToggle, arg);

    maxDTSpeed.x -= 10;
    maxDTSpeed.y -= 10;
    dt.maxSpeed = maxDTSpeed;
  }
}

void usercontrol(void) {
  vex::thread t1(DisplayLoop);

#pragma region Binds

  binding example_bind =
      binding(Controller1.ButtonR1, Controller1.ButtonR2, bExample, 100, true);

#pragma endregion

  while (1) {
    Vector2 target =
        Vector2(Controller1.Axis3.position(), Controller1.Axis2.position());
    dt.Drive(target);

#pragma region BindCall

    example_bind.RunCtrl();

#pragma endregion
    
    ControlDriveSpeed();

    this_thread::sleep_for(20);
  }
  t1.interrupt();
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  while (true) {

    if (!rendered) {
      DisplayImg();
    }

    wait(100, msec);
  }
}
