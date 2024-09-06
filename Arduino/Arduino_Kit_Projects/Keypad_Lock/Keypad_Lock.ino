#include <Servo.h>
#include <Key.h>
#include <Keypad.h>

// Keypad variables
const int ROW_COUNT = 4;
const int COL_COUNT = 3;
const char KEY_LAYOUT[ROW_COUNT][COL_COUNT]
	= {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
const byte ROW_PIN[ROW_COUNT] = {A2, A3, A4, A5};
const byte COL_PIN[COL_COUNT] = {4, 3, 2};

// Input components
Servo mainServo;
Keypad mainKeypad = Keypad(makeKeymap(KEY_LAYOUT), ROW_PIN, COL_PIN, ROW_COUNT, COL_COUNT);
const int switchPin = A0;

// Output components
const int greenLED = 5;
const int yellowLED = 6;
const int redLED = 9;

// Digital variables
const int PASSWORD_LEN = 4;
int password[PASSWORD_LEN];
int userEntry[4];
char keyPress;
const int FAST_BLINK = 125;
const int MED_BLINK = 250;
bool locked = false;
int switchVal;
int led[1] = {};

void setup() {
	pinMode(switchPin, INPUT);
	pinMode(greenLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);
	pinMode(redLED, OUTPUT);
	mainServo.attach(11);
	Serial.begin(9600);
	
	// Set the password
	digitalWrite(greenLED, HIGH);
	digitalWrite(yellowLED, HIGH);
	digitalWrite(redLED, HIGH);
	Serial.print("Waiting for 4 key password entry: ");
	
	int leds[3] = {greenLED, yellowLED, redLED};
	userKeypadEntry(leds, 3, true);
	for(int i = 0; i < 4; i++) {
		password[i] = userEntry[i];
	}
	
	// Flash all LEDs to indicate password initialization
	flashLEDs(leds, 3, 2, MED_BLINK, false);
	Serial.println("\nPassword set");
	
	digitalWrite(greenLED, HIGH);
	mainServo.write(0);
	Serial.println("Servo: unlocked");
}

void loop() {
	if(!locked) {
		switchVal = digitalRead(switchPin);
		if(switchVal == HIGH) {
			// If switch is engaged, power redLED and set
			// servo to "locked" position
			locked = true;
			digitalWrite(greenLED, LOW);
			digitalWrite(redLED, HIGH);
			mainServo.write(90);
			Serial.print("Servo: locked");
			delay(1000);
		}
	}
	if(locked) {
		// Wait for user entry and then check that password
		led[0] = yellowLED;
		userKeypadEntry(led, 1, false);
		if(!checkKeyEntry(userEntry, password)) {
			led[0] = redLED;
			flashLEDs(led, 1, 2, MED_BLINK, true);
			Serial.println("Incorrect key entry");
		} else {
			locked = false;
			digitalWrite(redLED, LOW);
			flashLEDs(led, 1, 2, MED_BLINK, false);
			digitalWrite(greenLED, HIGH);
			mainServo.write(0);
			delay(10);
			Serial.println("Servo: unlocked");
		}
	}
}

/**
 * User enters a 4 character sequence in the keypad.
 **/
void userKeypadEntry(int leds[], int arrLength, bool endHigh) {
	for(int i = 0; i < 4; i++) {
		keyPress = NO_KEY;
		while(keyPress == NO_KEY) {
			keyPress = mainKeypad.getKey();
		}
		userEntry[i] = keyPress;
		flashLEDs(leds, arrLength, 1, FAST_BLINK, endHigh);
		Serial.print(keyPress);
	}
}

/**
 * Checks the keypad entry to the set password.
 **/
bool checkKeyEntry(int entry[], int password[]) {
	for(int i = 0; i < 4; i++) {
		if(entry[i] != password[i]) {
			return false;
		}
	}
	
	return true;
}

/**
 * Flashes the specified leds a certain number of times
 **/
void flashLEDs(int leds[], int arrLength, int count, int blinkSpeed, bool endHigh) {
	count += endHigh;
	while(count > 0) {
		for(int i = 0; i < arrLength; i++) {
			digitalWrite(leds[i], HIGH);
		}
		delay(blinkSpeed);
		
		if(count == 1 && endHigh) {
			break;	
		} 
		
		for(int i = 0; i < arrLength; i++) {
			digitalWrite(leds[i], LOW);
		}
		delay(blinkSpeed);
		
		count--;
	}
}