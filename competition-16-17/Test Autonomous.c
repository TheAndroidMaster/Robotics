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

task main() {

	SensorValue[encoder3] = 0;
	SensorValue[encoder4] = 0;

	//reset arm position
	while (!SensorValue(touchSensor3) || !SensorValue(touchSensor4)) {
		if (!SensorValue(touchSensor3)) {
			motor[armMotor3] = -70;
		} else motor[armMotor3] = 0;

		if (!SensorValue(touchSensor4)) {
			motor[strongArm] = 70;
			motor[strongArm2] = 70;
		} else {
			motor[strongArm] = 0;
			motor[strongArm2] = 0;
		}
	}

	//should be holding a star - grab it
	motor[klaw1] = 100;
	motor[klaw2] = 100;

	//move forwards to the fence while raising the arm
	for (int i = 127; i >= 0; i--) {
		motor[leftMotor] = -i;
		motor[rightMotor] = -i;
		sleep(i * 3);

		motor[armMotor3] = SensorValue(encoder3) + 175;

		motor[strongArm] = (3.5 * SensorValue(encoder4)) - 360;
		motor[strongArm2] = (3.5 * SensorValue(encoder4)) - 360;
	}

	//drop the star
	motor[klaw1] = -127;
	motor[klaw2] = -127;

	//wait for two seconds for the star to drop, then stop the motors
	sleep(2000);
	motor[klaw1] = 0;
	motor[klaw2] = 0;
}