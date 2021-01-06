#ifndef BetterMotor_H
#define BetterMotor_H

#include "vex.h"
#include <iostream>
#include <math.h>
#include <string.h>

// ---
// This code aims to improve motor control by smoothing acceleration and decceleration
// ---

using namespace vex;

class BetterMotor {
  private:
    motor _motor;
    float speed = 0;
  public:
    BetterMotor(motor m) : _motor(m){
      _motor = m;
    }

    float maxSpeed = 100;
    
    float modSpeed = 20;
    float stoppingTreshold = 5;

    void Drive(float target){
      float increaseBy = (target - speed) / modSpeed;
      speed += increaseBy;

      if (speed < stoppingTreshold && speed > -stoppingTreshold && target < stoppingTreshold && target > -stoppingTreshold) {
        return Stop();
      }

      _motor.setVelocity(speed * (maxSpeed / 100), percent);
      _motor.spin(forward);
    }

    void Stop(){
      _motor.stop();
    }
};
#endif