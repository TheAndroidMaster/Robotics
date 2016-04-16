#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           armMotor1,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           armMotor2,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           armMotor3,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           rotateMotor,   tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//MOTORS:
//armMotor1 = bottom motor
//armMotor2 = moddle motor
//armMotor3 = claw tilting motor
//rotateMotor = arm rotating motor
//rightMotor and leftMotor are drive motors

//SENSORS:
//encoder2 = middle encoder
//encoder3 = bottom encoder

#define DEFAULT_SPEED 100
#define MAX_SPEED 127
#define MAX_VALUE 32000

void pop();

void move1(int count, int speed, int wait, int increase);
void move2(int count, int speed, int wait, int increase);
void move3(int count, int speed, int wait, int increase);

void rotate(int count, int speed, int wait, int increase);

int value = 0;

int max;

task main() {  	
  SensorValue[encoder2] = 0;
  SensorValue[encoder3] = 0;
    
  while (true) {
    //drive motors  	
  	motor[rightMotor] = ((-1 * vexRT[Ch1]) + vexRT[Ch2]) / 1.666;
    motor[leftMotor] = (vexRT[Ch1] + vexRT[Ch2]) / 1.666;
    
    //arm motors
    int motor1 = 0;
    int motor2 = 0;
    int motor3 = 0;
     
    if (vexRT[Btn5U] == 1) {
        //move arm back
        motor3 = DEFAULT_SPEED / 4;
    } else if (vexRT[Btn5D] == 1) {
        //move arm forward
        motor3 = -1 * (DEFAULT_SPEED / 4);
    } else {
        //move arm in accordance with left joystick
        //set value to left joystick up/down
        if (value + (vexRT[Ch3] / 2) > 0 && abs(value + (vexRT[Ch3] / 2)) < MAX_VALUE) value += vexRT[Ch3] / 2;
        
        //set motor speed based on value
        if (value > SensorValue[encoder2]) {
            motor2 = value - SensorValue[encoder2];
        } else {
            motor2 = -1 * (SensorValue[encoder2] - value);
        }
        
        if (value > SensorValue[encoder3]) {
            motor3 = value - SensorValue[encoder3];
        } else {
            motor3 = -1 * (SensorValue[encoder3] - value);
        }
        
        motor[rotateMotor] = vexRT[Ch4];
        
        motor3 = (motor2 + motor1) / 2;
    }
    
    motor[armMotor1] = motor1;
    motor[armMotor2] = motor2;
    motor[armMotor3] = motor3;
    
    if (vexRT[Btn7U]) {
        pop();
    } else if (vexRT[Btn7D]) {
        rotate(50, -50, 2, 1);
        rotate(50, 50, 2, -1);
        rotate(50, -50, 2, 1);
        rotate(50, 50, 2, -1);
        move1(50, MAX_SPEED, 5, -1);
        move1(50, -1 * MAX_SPEED, 5, 1);
    }
  }
}

void pop() {
    motor[armMotor2] = MAX_SPEED;
    wait1Msec(50);
    motor[armMotor2] = -1 * MAX_SPEED;
    wait1Msec(50);
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
