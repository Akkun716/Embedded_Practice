/** 
 * The program moves the arm of a servo motor using a potentiometer.
 * The servo arm rests to the right at max resistance and to the left
 * with no resistance.
 */

// Using the servo library to gain control of the servo motor
#include <Servo.h>

// Pin A0: potentiometer (pot)
int potPin = A0;
Servo myServo;
int potValue;
int angle;

void setup() {
	// Pin 9: servo motor
	myServo.attach(9);
	
	Serial.begin(9600);
}

void loop() {

	potValue = analogRead(potPin);
	Serial.print("Pot value: ");
	Serial.print(potValue);
	
	// Servo range of motion is 180 degrees, so must map potValue from
	// voltage to angle
	angle = map(potValue, 0, 1023, 0, 179);
	Serial.print("\nAngle value: ");
	Serial.print(angle);
	
	myServo.write(angle);
	delay(15);
}