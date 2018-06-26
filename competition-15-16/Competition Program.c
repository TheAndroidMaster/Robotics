////////////////////////////////////////////////////////////////////////////////
//  Copyright 2018 James Fenn
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
////////////////////////////////////////////////////////////////////////////////

#pragma config(Sensor, dgtl1,  sonarSensor,    sensorSONAR_cm)
#pragma config(Sensor, dgtl3,  touchSensorArm, sensorTouch)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           armMotor,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           leftIntake,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           rightIntake,   tmotorServoStandard, openLoop, reversed)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

const int speed = 127;
const int turn = 100;

task autonomous() {
	while(SensorValue(touchSensorArm) == 0) {
		motor[armMotor] = -127; 
	}
	motor[armMotor] = 0;

	while(true) {
		if (SensorValue(touchSensorArm) == 0) motor[armMotor] = -1 * speed;
		else motor[armMotor] = 0;

		if (SensorValue(sonarSensor) < 80 && SensorValue(sonarSensor) > 10) {
			motor[leftIntake] = -1 * speed;
			motor[rightIntake] = -1 * speed;
			} else if (SensorValue(sonarSensor) < 80) {
			motor[leftMotor] = -1 * turn;
			motor[rightMotor] = turn;
			} else {
			motor[leftMotor] = speed;
			motor[rightMotor] = speed;
		}
	}

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

int r = 0;
int l = 0;
int a = 0;

task usercontrol() {
	// User control code here, inside the loop

	while (true) {
		// This is the main execution loop for the user control program. Each time through the loop
		// your program should update motor + servo values based on feedback from the joysticks.
		if (vexRT[Btn5U] == 1) {
			//this section is designated for whenever the user wants the robot to move quickly, but not be hard to control
			//to make it easier to drive in a straight line, the left/right joystick values have been divided by two
			r = vexRT[Ch2] + (-1 * (vexRT[Ch1] / 2));
			l = vexRT[Ch2] + (vexRT[Ch1] / 2);

			// these if/else statements make sure the motor speed is at an acceptable value
			if (r > 127) r = 127;
			else if (r < -127) r = -127;
			if (l > 127) l = 127;
			else if (l < -127) l = -127;
			} else {
			//this section is designated for whenever the user wants a quick turning ratio, but some more precision of the location of the robot
			r = (vexRT[Ch2] + (-1 * vexRT[Ch1])) / 3;
			l = (vexRT[Ch2] + vexRT[Ch1]) / 3;
		}

		//sets the motor speed to the values set in the above section
		motor[rightMotor] = r;
		motor[leftMotor] = l;

		//this section controls the intake motors on the front of the robot
		if (vexRT[Btn6U]) {
			//up is pressed, intake a ball
			motor[leftIntake] = 127;
			motor[rightIntake] = 127;
			} else if (vexRT[Btn6D]) {
			//down is pressed, push the ball outwards
			motor[leftIntake] = -127;
			motor[rightIntake] = -127;
			} else {
			//nothing is pressed, don't move the motors
			motor[leftIntake] = 0;
			motor[rightIntake] = 0;
		}

		//this decides the speed of the arm
		a = vexRT[Ch3];
		if (SensorValue(touchSensorArm) == 1 && a < 0) {
			//the arm is already lowered, don't move it any lower or we might break the touch sensor
			a = 0;
		}
		motor[armMotor] = a;
	}
}


