#ifndef BetterDrivetrain_H
#define BetterDrivetrain_H

#include "util.h"
#include "vex.h"
#include <iostream>

// ---
// This code aims to improve drivetrain control by smoothly accelerating do a desired speed and smoothly decellerating
// ---

class BetterDrivetrain {
  private:
    Vector2 speed = Vector2(0, 0);
    
  public:
    Vector2 modSpeed = Vector2(20, 20);
    Vector2 maxSpeed = Vector2(100, 100);
    float stoppingThreshold = 5;

    BetterDrivetrain(){
    }

    void Stop() { Drivetrain.stop(); }

    void Drive(Vector2 target) {
      float xIncreaseBy = (target.x - speed.x) / modSpeed.x;
      speed.x < target.x ? speed.x += xIncreaseBy : speed.x += xIncreaseBy;

      float yIncreaseBy = (target.y - speed.y) / modSpeed.y;
      speed.y < target.y ? speed.y += yIncreaseBy : speed.y += yIncreaseBy;

      if (speed.within(stoppingThreshold) && target.within(stoppingThreshold)) {
        return Stop();
      }
      LeftDriveSmart.setVelocity(speed.x * (maxSpeed.x / 100), percent);
      LeftDriveSmart.spin(forward);

      RightDriveSmart.setVelocity(speed.y * (maxSpeed.y / 100), percent);
      RightDriveSmart.spin(forward);
    };
};

#endif