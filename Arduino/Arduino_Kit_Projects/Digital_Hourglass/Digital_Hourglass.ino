/**
 * The program is a timer that uses six LEDs as time markers until all LEDs are
 * illuminated. The timer can be reset with the use of a switch
 */

const int switchPin = 8;
// prevTime records the previous time the LEDs changed
unsigned long prevTime = 0;
int switchState = 0;
int prevSwitchState = 0;
// led keeps track of which led will be turned on next
int led = 2;
// delayInterval is the interval between each led lighting
long delayInterval = 4000;
// lastLED is an indicator the timer is on its last LED before time's up
int lastLED = 0;

void setup() {
	for(int i = 2; i < 8; i++) {
		pinMode(i, OUTPUT);
	}
	
	pinMode(switchPin, INPUT);
}

void loop() {
	unsigned long currentTime = millis();
	
	// If the time passed since prevTime has exceeded the lighting interval...
	if(currentTime - prevTime > delayInterval && led < 8) {
		prevTime = currentTime;
		digitalWrite(led, HIGH);
		led++;
		
		// If on last led and the indicator hasn't activated yet...
		if(led == 7 && !lastLED) {
			lastLED = 1;
			LEDAnimation();
		}
	}
	
	switchState = digitalRead(switchPin);
	
	if(switchState != prevSwitchState) {
		// Turn all LEDs off and prepare the first LED
		for(int i = 2; i < 8; i++) {
			digitalWrite(i, LOW);
		}
		
		led = 2;
		lastLED = 0;
		prevTime = currentTime;
	}
	
	prevSwitchState = switchState;
	
}

void LEDAnimation() {
	flashLED(7);
}

// Simple lastLED pattern. Flashes the last LED 2 times
void flashLED(int led) {
	int delayTime = 500;
	for(int i = 0; i < 2; i++) {
		digitalWrite(led, LOW);
		delay(delayTime);
		digitalWrite(led, HIGH);
		delay(delayTime);
		digitalWrite(led, LOW);
	}
}