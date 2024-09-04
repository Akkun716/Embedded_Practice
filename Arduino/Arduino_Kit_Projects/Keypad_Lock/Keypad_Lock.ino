#include <Servo.h>
#include <Keypad.h>

// Input components
Servo lockingServo;
const int lockSwitch = 2;
const int keypad = A0;
// Output components
const int greenLED = 3;
const int yellowLED = 4;
const int redzLED = 5;

void setup() {
	lockServo.attach(9);
	pinMode(lockSwitch, INPUT);
	pinMode(keypad, INPUT);
	pinMode(greenLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);
	pinMode(redLED, OUTPUT);
	
	digitalWrite(greenLED, HIGH);
	myServo.write(0);
	Serial.print("Servo: unlocked");
}

void loop() {
	if(!locked) {
		switchVal = digitalRead(switchPin);
		if(switchVal == HIGH) {
			// If switch is not engaged, power redLED and set
			// servo to "locked" position
			locked = true;
			digitalWrite(greenLED, LOW);
			digitalWrite(redLED, HIGH);
			myServo.write(90);
			Serial.print("Servo: locked");
			delay(1000);
		}
	}
	if(locked) {
		knockVal = analogRead(piezo);
		if(numOfKnocks < 3 && knockVal > 0) {
			if(checkForKnocks(knockVal)) {
				numOfKnocks++;
			}
		}
		if(numOfKnocks >= 3) {
			locked = false;
			myServo.write(0);
			delay(10);
			digitalWrite(redLED, LOW);
			digitalWrite(greenLED, HIGH);
			Serial.print("Servo: unlocked");
		}
	}
}