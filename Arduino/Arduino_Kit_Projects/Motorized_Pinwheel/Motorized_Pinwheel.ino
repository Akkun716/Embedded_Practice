/**
 * The program activates a DC motor with a push-button. Hold down to power
 * the motor, release to turn it off.
 */

const int switchPin = 2;
const int motorPin = 9;
int switchState = 0;

void setup() {
	pinMode(switchPin, INPUT);
	pinMode(motorPin, OUTPUT);
}

void loop() {
	switchState = digitalRead(switchPin);

	if(switchState == HIGH) {
		digitalWrite(motorPin, HIGH);
	} else {
		digitalWrite(motorPin, LOW);
	}
}