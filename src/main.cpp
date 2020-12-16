#include "custom/auton/auton.h"
#include "custom/better-drivetrain.h"
#include "custom/better-motor.h"
#include "custom/image/image-loader.h"
#include "vex.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vex_task.h>

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// Drivetrain           drivetrain    1, 2, 3, 4
// Arm                  motor         5
// Claw                 motor         6
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;

util::side curSide = none;

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

void LeftAuton() {
  top.spinFor(forward, 180, deg);
  // TurnToTile(Vector2(2, 0));
  Drivetrain.driveFor(forward, -350, mm);
  for (int i = 0; i < 10; i++) {
    bTop.Drive(-100);
    bBack.Drive(-100);
  }

  wait(2, sec);
  for (int i = 0; i < 25; i++) {
    bTop.Drive(0);
    bBack.Drive(0);
  }
}

void RightAuton() {
  top.spinFor(forward, 180, deg);
  TurnToTile(Vector2(1, 0));
  Drivetrain.driveFor(forward, -300, mm);
  for (int i = 0; i < 10; i++) {
    bTop.Drive(-100);
    bBack.Drive(-100);
  }
  wait(2, sec);
  for (int i = 0; i < 25; i++) {
    bTop.Drive(0);
    bBack.Drive(0);
  }
}

void autonomous(void) {
  curSide = util::left;
  // Initialise the auton code with the selected side
  Initialise(curSide);

  // Code for if they are on the left or right side
  if (curSide == util::left) {
    LeftAuton();
  } else if (curSide == util::right) {
    RightAuton();
  }
}

Vector2 maxDTSpeed = Vector2(40, 40);

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

void usercontrol(void) {

  vex::thread t1(DisplayLoop);
  bool front_toggle = false;
  int front_speed = 0;
  int toggle_wait = 0;
  int speed_wait = 0;
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

    // -- Alternate Drive Speeds
    if (Controller1.ButtonUp.pressing() && maxDTSpeed.x + 10 < 105 &&
        speed_wait > 10) {
      speed_wait = 0;
      maxDTSpeed.x += 10;
      maxDTSpeed.y += 10;
      dt.maxSpeed = maxDTSpeed;

    } else if (Controller1.ButtonDown.pressing() && maxDTSpeed.x - 10 > 5 &&
               speed_wait > 10) {
      speed_wait = 0;
      maxDTSpeed.x -= 10;
      maxDTSpeed.y -= 10;
      dt.maxSpeed = maxDTSpeed;
    }
    if (speed_wait < 15) {
      speed_wait++;
    }
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
