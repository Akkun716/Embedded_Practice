/**
 * The program is a timer that uses six LEDs as time markers until all LEDs are
 * illuminated. The timer can be reset with the use of a switch (a push button
 * switch in this case)
 */

const int switchPin = 8;
unsigned long prevTime = 0;
int switchState = 0;
int prevSwitchState = 0;
int led = 2;
long delayInterval = 600000;

void setup() {
	for(int i = 2; i < 8; i++) {
		pinMode(i, OUTPUT);
	}
	
	pinMode(switchPin, INPUT);
}

void loop() {
	unsigned long currentTime = millis();
	
	if(prevTime - currentTime > delayInterval) {
		prevTime = currentTime;
		digitalWrite(led, HIGH);
		led++;
		
		if(led == 7) {
			//for(int i = 0; i < 2; i++) {
			//	flashLed(led, 500);
			//}
		}
	}
	
	switchState = digitalRead(switchPin);
	
	if(switchState != prevSwitchState) {
		
		for(int i = 2; i < 8; i++) {
			digitalWrite(i, LOW);
		}
		led = 2;
		prevTime = currentTime;
	}
	
	prevSwitchState = switchState;
	
}

void flashLed(int led, int delayTime) {
	
	digitalWrite(led, LOW);
	delay(delayTime);
	digitalWrite(led, HIGH);
	delay(delayTime);
}