#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(Sensor, dgtl1,  encoder3,       sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  encoder4,       sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  touchSensor4,   sensorTouch)
#pragma config(Sensor, dgtl6,  touchSensor3,   sensorTouch)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           armMotor3,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           strongArm,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           strongArm2,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           klaw1,         tmotorVex269_MC29, openLoop)
#pragma config(Motor,  port6,           klaw2,         tmotorVex269_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

float getSpeed();
int motor3Value;
int strongArmValue;

bool klaw;

task main() {
  
	SensorValue[encoder3] = 0;
	SensorValue[encoder4] = 0;
  
	while (true) {
    
		motor[rightMotor] = (vexRT[Ch4] - vexRT[Ch3] - vexRT[Ch2] + vexRT[Ch1] + motor[rightMotor]) / 3;
		motor[leftMotor] = (-vexRT[Ch4] - vexRT[Ch3] - vexRT[Ch2] - vexRT[Ch1] + motor[rightMotor]) / 3;
    
		if (vexRT[Btn5U] && !SensorValue(touchSensor3)) {
			motor[armMotor3] = -getSpeed();
			motor3Value = SensorValue(encoder3);
		} else if (vexRT[Btn5D]) {
			motor[armMotor3] = getSpeed();
			motor3Value = SensorValue(encoder3);
		} else if (vexRT[Btn7U]) {
			motor[armMotor3] = SensorValue(encoder3) + 215;
			motor3Value = SensorValue(encoder3);
		} else {
			motor[armMotor3] = SensorValue(encoder3) - motor3Value;
		}
    
		if (vexRT[Btn6U]) {
			motor[strongArm] = -getSpeed();
			motor[strongArm2] = -getSpeed();
			strongArmValue = 3.5 * SensorValue(encoder4);
		} else if (vexRT[Btn6D] && !SensorValue(touchSensor4)) {
			motor[strongArm] = getSpeed();
			motor[strongArm2] = getSpeed();
			strongArmValue = 3.5 * SensorValue(encoder4);
		} else if (vexRT[Btn7U]) {
			motor[strongArm] = (3.5 * SensorValue(encoder4)) - (3.5 * 118);
			motor[strongArm2] = (3.5 * SensorValue(encoder4)) - (3.5 * 118);
			strongArmValue = 3.5 * SensorValue(encoder4);
		} else {
			motor[strongArm] = (3.5 * SensorValue(encoder4)) - strongArmValue;
			motor[strongArm2] = (3.5 * SensorValue(encoder4)) - strongArmValue;
		}
    
		if (vexRT[Btn7R]) {
			motor[klaw1] = 127;
			motor[klaw2] = 127;
			klaw = true;
		} else if (vexRT[Btn7L]) {
			motor[klaw1] = -127;
			motor[klaw2] = -127;
			klaw = false;
		} else {
			if (klaw) {
				motor[klaw1] = 5;
				motor[klaw2] = 5;
			} else {
				motor[klaw1] = -5;
				motor[klaw2] = -5;
			}
		}
    
		displayLCDNumber(0, 0, SensorValue(encoder3));
		displayLCDNumber(0, 6, SensorValue(encoder4));
    
		if (vexRT[Btn7D]) {
			while ((!SensorValue(touchSensor3) || !SensorValue(touchSensor4)) && vexRT[Btn7D]) {
				if (!SensorValue(touchSensor3)) {
					motor[armMotor3] = -70;
					motor3Value = SensorValue(encoder3);
				} else motor[armMotor3] = 0;
        
				if (!SensorValue(touchSensor4)) {
					motor[strongArm] = 70;
					motor[strongArm2] = 70;
					strongArmValue = 3.5 * SensorValue(encoder4);
				} else {
					motor[strongArm] = 0;
					motor[strongArm2] = 0;
				}
			}
      
			SensorValue[encoder3] = 0;
			SensorValue[encoder4] = 0;
		}
	} 
}

float getSpeed() {
	if (vexRT[Btn8R]) return 50;
	else return 127;
}
