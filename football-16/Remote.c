#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           armMotor1,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           armMotor2,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           armMotor3,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rotateMotor,   tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//armMotor1 = bottom motor
//armMotor2 = moddle motor
//armMotor3 = claw tilting motor
//rotateMotor = arm rotating motor
//rightMotor and leftMotor are drive motors

#define DEFAULT_SPEED 100
#define MAX_SPEED 127

void move1(int count, int speed, int wait, int increase);
void move2(int count, int speed, int wait, int increase);
void move3(int count, int speed, int wait, int increase);

void rotate(int count, int speed, int wait, int increase);

task main() {  	
  while (true) {
    //drive motors  	
  	motor[rightMotor] = (-1 * vexRT[Ch1])) / 2;
    motor[leftMotor] = vexRT[Ch1]) / 2;
    
    //arm motors
    int motor1 = 0;
    int motor2 = 0;
    int motor3 = 0;
     
    if (vexRT[Btn5U] == 1) {
        //move arm back
        motor3 = DEFAULT_SPEED / 7;
    } else if (vexRT[Btn5D] == 1) {
        //move arm forward
        motor3 = -1 * (DEFAULT_SPEED / 7);
    } else {
        //move arm in accordance with left joystick
        motor[armMotor2] = vexRT[Ch3] / 2;
        motor[armMotor1] = vexRT[Ch3] / 2;
        motor[rotateMotor] = vexRT[Ch4];
        
        motor3 = (motor2 + motor1) / 2;
    }
    
    motor[armMotor1] = motor1;
    motor[armMotor2] = motor2;
    motor[armMotor3] = motor3;
    
    if (vexRT[Btn7U]) {
        move2(50, MAX_SPEED - 50, 50, 1);
    } else if (vexRt[Btn7D]) {
        rotate(50, -50, 50, 1);
        rotate(50, 50, 50, -1);
        rotate(50, -50, 50, 1);
        rotate(50, 50, 50, -1);
        move1(50, MAX_SPEED, 25, -1);
        move1(50, -1 * MAX_SPEED, 25, 1);
    }
  }
}

void move1(int count, int speed, int wait, int increase) {
    for (int i = 0; i < count; i++) {
        motor[armMotor1] = speed + (increase * i);
        wait1Msec(wait);
    }
}

void move2(int count, int speed, int wait, int increase) {
    for (int i = 0; i < count; i++) {
        motor[armMotor2] = speed + (increase * i);
        wait1Msec(wait);
    }
}

void move3(int count, int speed, int wait, int increase) {
    for (int i = 0; i < count; i++) {
        motor[armMotor3] = speed + (increase * i);
        wait1Msec(wait);
    }
}

void rotate(int count, int speed, int wait, int increase) {
    for (int i = 0; i < count; i++) {
        motor[armMotor1] = speed + (increase * i);
        wait1Msec(wait);
    }
}


