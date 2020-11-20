#include "vex.h"
#include "custom/better-drivetrain.h"
#include "custom/better-motor.h"
#include "custom/image/image-loader.h"
#include <iostream>

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// Drivetrain           drivetrain    1, 2, 3, 4
// Arm                  motor         5
// Claw                 motor         6
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;

competition Competition;

BetterDrivetrain dt = BetterDrivetrain();
BetterMotor bArm = BetterMotor(Arm);
BetterMotor bClaw = BetterMotor(Claw);

void pre_auton(void)
{
	vexcodeInit();
	
	dt.maxSpeed = Vector2(66, 66);
	dt.modSpeed = Vector2(20, 20);

	bArm.maxSpeed = 100;
	bArm.modSpeed = 20;

	bClaw.maxSpeed = 100;
	bClaw.modSpeed = 20;
	DisplatImg();
}

void autonomous(void)
{
}

void usercontrol(void)
{
	while (1)
	{
		// -- Drivetrain Controll
		Vector2 target = Vector2(Controller1.Axis3.position(), Controller1.Axis2.position());
		dt.Drive(target);

		// -- Arm and Claw Control
		if (Controller1.ButtonR1.pressing())
		{
			bArm.Drive(100);
		}
		else if (Controller1.ButtonR2.pressing())
		{
			bArm.Drive(-100);
		}
		else
		{
			bArm.Drive(0);
		}

		if (Controller1.ButtonL2.pressing())
		{
			bClaw.Drive(100);
		}
		else if (Controller1.ButtonL1.pressing())
		{
			bClaw.Drive(-100);
		}
		else
		{
			bClaw.Drive(0);
		}

		// -- Alternate Drive Speeds
		if (Controller1.ButtonA.pressing())
		{
			dt.maxSpeed = Vector2(25, 25);
		}
		else if (Controller1.ButtonB.pressing())
		{
			dt.maxSpeed = Vector2(66, 66);
		}
		else if (Controller1.ButtonY.pressing())
		{
			dt.maxSpeed = Vector2(100, 100);
		}

		wait(20, msec);
	}
}

int main()
{
	Competition.autonomous(autonomous);
	Competition.drivercontrol(usercontrol);

	pre_auton();

	while (true)
	{
		wait(100, msec);
	}
}
