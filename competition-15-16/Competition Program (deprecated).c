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

#pragma config(Sensor, dgtl1,  sonarSensorFront, sensorSONAR_cm)
#pragma config(Sensor, dgtl3,  sonarSensorRight, sensorSONAR_cm)
#pragma config(Sensor, dgtl5,  sonarSensorBack, sensorSONAR_cm)
#pragma config(Sensor, dgtl7,  sonarSensorLeft, sensorSONAR_cm)
#pragma config(Sensor, dgtl9,  sonarSensorBalls, sensorSONAR_cm)
#pragma config(Sensor, dgtl11, touchSensorBalls, sensorTouch)
#pragma config(Motor,  port1,           rightMotor,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightIntake,   tmotorServoStandard, openLoop, reversed)
#pragma config(Motor,  port3,           leftIntake,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           rightThrower,  tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           leftThrower,   tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          leftMotor,     tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(15)
#pragma userControlDuration(105)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

//This program is deprecated because, well, the ultrasonic sensors weren't working properly.
//Keeping this program so I can scream at my horrible code later and possibly use it for something (but probably not).

bool isIntake;
bool intakeHold;

bool isThrower;
bool throwerHold;

//autonomous-specific variables
int x = 0;
int y = 0;
int x2 = 0;
int y2 = 0;
int rotation = 0;
void updatePos();

void rotate(int newRotation);
void move(int newX, int newY);

int ballX[99];
int ballY[99];
int ballCount = 0;
int visitedBallX[99];
int visitedBallY[99];
int visitedBallCount = 0;
void notifyBall(int x, int y);
int ballWidth = 12;
void intake();
void throw();

void reset();

//remote-specific variables
void toggleThrower();
void toggleIntake();

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

task autonomous()
{
  // .....................................................................................
  // Insert user code here.
  // .....................................................................................

	while(true) { 
        reset();
        if (SensorValue(touchSensorBalls) == 1) {
            //balls full, throw them at net
            throw();
        } else if (ballCount > 0) {
            //go get a ball
            move(ballX[ballCount - 1], ballY[ballCount - 1]);
            intake();
        } else {
            //nothing important, change position
            while(ballCount == 0) {
                if (SensorValue(sonarSensorFront) > 10) {
                    motor[rightMotor] = 100;
                    motor[leftMotor] = 100;
                } else {
                    reset();
                    if (rotation == 270) rotate(rotation - 90);
                    else rotate(rotation + 90);
                }
                updatePos();
            }
        }
	}
}

void updatePos() {
    int newX, newY, newX2, newY2;
    switch(rotation) {
        case 0:
            newX = SensorValue(sonarSensorLeft);
            newY = SensorValue(sonarSensorBack);
            newX2 = SensorValue(sonarSensorRight);
            newY2 = SensorValue(sonarSensorFront);
            break;
        case 90:
            newX = SensorValue(sonarSensorBack);
            newY = SensorValue(sonarSensorRight);
            newX2 = SensorValue(sonarSensorFront);
            newY2 = SensorValue(sonarSensorLeft);
            break;
        case 180:
            newX = SensorValue(sonarSensorRight);
            newY = SensorValue(sonarSensorFront);
            newX2 = SensorValue(sonarSensorLeft);
            newY2 = SensorValue(sonarSensorBack);
            break;
        case 270:
            newX = SensorValue(sonarSensorFront);
            newY = SensorValue(sonarSensorLeft);
            newX2 = SensorValue(sonarSensorBack);
            newY2 = SensorValue(sonarSensorRight);
            break;
        default:
            //cannot accurately determine position, return
            return;
    }
    //robot will not go far without checking position, so it's safe to assume above (ballWidth + 5) difference is either a ball or another robot
    if (x - newX > ballWidth + 5) {
        notifyBall(x - newX, newY);
        newX = x;
    }
    if (y - newY > ballWidth + 5) {
        notifyBall(newX, y - newY);
        newY = y;
    }
    if (x2 - newX2 > ballWidth + 5) {
        notifyBall((x2 + newX) - newX2, newY2);
        newX2 = x2;
    }
    if (y2 - newY2 > ballWidth + 5) {
        notifyBall(newX2, (y2 + newY) - newY2);
        newY2 = y2;
    }
    
    x = newX;
    y = newY;
    x2 = newX2;
    y2 = newY2;
}

void reset() {
    motor[rightMotor] = 0;
    motor[leftMotor] = 0;
    motor[rightIntake] = 0;
    motor[leftIntake] = 0;
    motor[rightThrower] = 0;
    motor[leftThrower] = 0;
}

void rotate(int newRotation) {
    //check not current rotation
    if (rotation == newRotation) return;    
    //rotate to degrees
    //first step: get current x and y       
    updatePos();
    int startX = x;
    int startY = y;
    int startX2 = x2;
    int startY2 = y2;
    while (newRotation - rotation >= 180) {
        //turning left will be quicker
        do {
            motor[rightMotor] = 100;
            motor[leftMotor] = -100;
            updatePos();
            //distances should be within 3cm (difference in sensor placement on robot) when it is finished turning
        } while (((abs(startX - x) > 3) || (abs(startY - y) > 3)) && ((abs(startX2 - x2) > 3) || (abs(startY2 - y2) > 3)));
        reset();
        //subtract from rotation because it is turning left
        rotation -= 90;
    } 
    while (newRotation - rotation < 180) {
        //turning right is fastest
        do {
            motor[rightMotor] = -100;
            motor[leftMotor] = 100;
            updatePos();
            //distances should be within 3cm (difference in sensor placement on robot) when it is finished turning
        } while (((abs(startX - x) > 3) || (abs(startY - y) > 3)) && ((abs(startX2 - x2) > 3) || (abs(startY2 - y2) > 3)));
        reset();
        //add to rotation because it is turning right
        rotation += 90;
    }    
}

void move(int newX, int newY) {
    if (newX > x) {
        //move right
        rotate(90);
        while(newX > x) {
            motor[rightMotor] = 100;
            motor[leftMotor] = 100;
            updatePos();
            if (SensorValue(sonarSensorBalls) <= 3) {
                ballCount++;
                intake();
                //return to check the intake isn't full - if it isn't the current movement should resume anyway
                return;
            }
        }
    }
    else if (newX < x) {
        //move left 
        rotate(270);
        while(newX < x) {
            motor[rightMotor] = 100;
            motor[leftMotor] = 100;
            updatePos();
            if (SensorValue(sonarSensorBalls) <= 3) {
                ballCount++;
                intake();
                //return to check the intake isn't full - if it isn't the current movement should resume anyway
                return;
            }
        }
    }
    if (newY > y) {
        //move forwards 
        rotate(0);
        while(newY > y) {
            motor[rightMotor] = 100;
            motor[leftMotor] = 100;
            updatePos();
            if (SensorValue(sonarSensorBalls) <= 3) {
                ballCount++;
                intake();
                //return to check the intake isn't full - if it isn't the current movement should resume anyway
                return;
            }
        }
    }
    else if (newY < y) {
        //move backwards 
        rotate(180);
        while(newY < y) {
            motor[rightMotor] = 100;
            motor[leftMotor] = 100;
            updatePos();
            if (SensorValue(sonarSensorBalls) <= 3) {
                ballCount++;
                intake();
                //return to check the intake isn't full - if it isn't the current movement should resume anyway
                return;
            }
        }        
    }
    reset();
}

void notifyBall(int x, int y) {
    //add coordinates to array
    for (int i = 0; i < visitedBallCount; i++) {
        //check ball wasn't already picked up, avoids possible confusion of ball with another robot
        if ((visitedBallX[i] - x > ballWidth || visitedBallX[i] - x < ballWidth) && (visitedBallY[i] - y > ballWidth || visitedBallY[i] - y < ballWidth)) {
            //ball is too similar, return
            return;
        }
    }
    ballX[ballCount] = x;
    ballY[ballCount] = y;
    ballCount++;
}

void intake() {
    if (SensorValue(touchSensorBalls) == 1) return;
    //intake balls
    int count;
    while(SensorValue(touchSensorBalls) == 0) {
        if (SensorValue(sonarSensorBalls) <= 3) {
            motor[rightIntake] = 100;
            motor[leftIntake] = 100;
            count++;
        } else {
            break;
        }
        motor[rightMotor] = 5;
        motor[leftMotor] = 5;
        motor[leftThrower] = -10;
        motor[rightThrower] = -10;
        if(count > 1) break;
    }
    reset();
    ballCount--;
    visitedBallX[visitedBallCount] = x;
    visitedBallY[visitedBallCount] = y;
    visitedBallCount++;
}

void throw() {
    //throw all balls
    motor[rightThrower] = 127;
    motor[leftThrower] = 127;
    move(50, 50);
    rotate(0);
    motor[rightIntake] = 127;
    motor[leftIntake] = 127;
    sleep(5);
    reset();
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	// User control code here, inside the loop

	while (true)
	{
	  // This is the main execution loop for the user control program. Each time through the loop
	  // your program should update motor + servo values based on feedback from the joysticks.

	  // .....................................................................................
	  // Insert user code here. This is where you use the joystick values to update your motors, etc.
	  // .....................................................................................
	
		motor[leftMotor] = vexRT[Ch3]; 
		motor[rightMotor] = vexRT[Ch2]; 

		if (vexRT[Btn5U] == 1 && !throwerHold) {
			//toggle thrower
			toggleThrower();
			throwerHold = true;
		} else throwerHold = false;
		
		if (vexRT[Btn5D] == 1 && !intakeHold) {
			//toggle thrower
			toggleIntake();
			intakeHold = true;
		} else intakeHold = false;
	}
}

void toggleIntake() {
	if (isIntake) {
		//turn motor off
		motor[rightIntake] = 0;
		motor[leftIntake] = 0;
		isIntake = false;
	} else {
		//turn motor on
		motor[rightIntake] = 100;
		motor[leftIntake] = 100;
		isIntake = true;
	}
}

void toggleThrower() {
	if (isThrower) {
		//turn motor off
		motor[rightThrower] = 0;
        motor[leftThrower] = 0;
		isThrower = false;
	} else {
		//turn motor on
		motor[rightThrower] = 127;
        motor[leftThrower] = 127;
		isThrower = true;
	}
}


