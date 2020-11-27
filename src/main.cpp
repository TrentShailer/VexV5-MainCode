#include "custom/auton/auton-selector.h"
#include "custom/auton/auton.h"
#include "custom/better-drivetrain.h"
#include "custom/better-motor.h"
// #include "custom/image/image-loader.h"
#include "vex.h"
#include <iostream>

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// Drivetrain           drivetrain    1, 2, 3, 4
// Arm                  motor         5
// Claw                 motor         6
// ---- END VEXCODE CONFIGURED DEVICES ----

// Sensors:
// Bottom toggles mid
// Second toggles mid and top
// Final toggles second

using namespace vex;

competition Competition;

// Variables for better control
BetterDrivetrain dt = BetterDrivetrain();
BetterMotor bFront = BetterMotor(front);
BetterMotor bMid = BetterMotor(mid);
BetterMotor bTop = BetterMotor(top);
BetterMotor bBack = BetterMotor(back);

void pre_auton(void) {
  vexcodeInit();

  bFront = BetterMotor(front);
  bMid = BetterMotor(mid);
  bTop = BetterMotor(top);
  bBack = BetterMotor(back);

  // Set the max speed for the drivetrain and the modifier for the increase rate
  dt.maxSpeed = Vector2(66, 66);
  dt.modSpeed = Vector2(10, 10);

  bFront.maxSpeed = 100;
  bFront.modSpeed = 10;

  bMid.maxSpeed = 100;
  bMid.modSpeed = 10;

  bTop.maxSpeed = 100;
  bTop.modSpeed = 10;

  bBack.maxSpeed = 100;
  bBack.modSpeed = 10;
}

void LeftAuton() { MoveTo(Vector2(1, 1)); }

void RightAuton() { TurnToTile(Vector2(5, 0)); }

void autonomous(void) {
  // Initialise the auton code with the selected side
  Initialise(curSide);

  // Code for if they are on the left or right side
  if (curSide == util::left) {
    LeftAuton();
  } else if (curSide == util::right) {
    RightAuton();
  }
}

void usercontrol(void) {
  bool front_toggle = false;
  int front_speed = 0;
  int toggle_wait = 0;
  bool autonToggle = false;
  while (1) {
    // -- Drivetrain Control
    Vector2 target =
        Vector2(Controller1.Axis3.position(), Controller1.Axis2.position());
    dt.Drive(target);

    if (Controller1.ButtonL1.pressing() && toggle_wait > 10) {
      toggle_wait = 0;
      front_toggle = !front_toggle;
      front_speed = 100;
    } else if (Controller1.ButtonL2.pressing() && toggle_wait > 10) {
      toggle_wait = 0;
      front_toggle = !front_toggle;
      front_speed = -100;
    }
    if (toggle_wait < 15) {
      toggle_wait++;
    }

    if (front_toggle) {
      bFront.Drive(front_speed);
      bMid.Drive(front_speed);
    } else {
      bFront.Drive(0);
      bMid.Drive(0);
    }

    if (Controller1.ButtonR1.pressing()) {
      bTop.Drive(100);
      bBack.Drive(100);
    } else if (Controller1.ButtonR2.pressing()) {
      bTop.Drive(-100);
      bBack.Drive(-100);
    } else {
      bTop.Drive(0);
      bBack.Drive(0);
    }

    if (Controller1.ButtonX.pressing() && !autonToggle) {
      autonToggle = true;
      // Initialise the auton code with the selected side
      Initialise(curSide);
      if (curSide == util::left) {
        LeftAuton();
      } else if (curSide == util::right) {
        RightAuton();
      }
    }
    // -- Alternate Drive Speeds
    if (Controller1.ButtonA.pressing()) {
      dt.maxSpeed = Vector2(25, 25);
    } else if (Controller1.ButtonB.pressing()) {
      dt.maxSpeed = Vector2(66, 66);
    } else if (Controller1.ButtonY.pressing()) {
      dt.maxSpeed = Vector2(100, 100);
    }

    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  curSide = util::right;
  while (true) {
    if (curSide == none) {
      // TODO - Fix button display
      // Render buttons for auton selection
      ButtonLoop(Competition);
      vex::task::sleep(7);
    } else {
      // if (!rendered) {
      //   DisplayImg();
      // }
      wait(100, msec);
    }
  }
}
