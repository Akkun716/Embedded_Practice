/****
 * This program utilizes a servo to simulate a locking mechanism.
 * A push switch is used to set the servo to "locked" and keypad
 * with a password set during the startup of the arduino to unlock.
 ****/

#include <Servo.h>
#include <Key.h>
#include <Keypad.h>

// Keypad variables
const int ROW_COUNT = 4;
const int COL_COUNT = 3;
const char KEY_LAYOUT[ROW_COUNT][COL_COUNT] = {
{'1', '2', '3'},
{'4', '5', '6'},
{'7', '8', '9'},
{'*', '0', '#'}
};
const byte ROW_PIN[ROW_COUNT] = {A2, A3, A4, A5};
const byte COL_PIN[COL_COUNT] = {4, 3, 2};

// Input components
Servo mainServo;
Keypad mainKeypad =
	Keypad(makeKeymap(KEY_LAYOUT), ROW_PIN, COL_PIN, ROW_COUNT, COL_COUNT);
const int switchPin = A0;

// Output components
const int greenLED = 5;
const int yellowLED = 6;
const int redLED = 9;

// Digital variables
// Set speeds for LED blinks
const int FAST_BLINK = 50;
const int MED_BLINK = 250;
const int PASSWORD_LEN = 4;
char password[PASSWORD_LEN];
char userEntry[PASSWORD_LEN];
bool locked = false;
int led[1] = {};

void setup() {
	pinMode(switchPin, INPUT);
	pinMode(greenLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);
	pinMode(redLED, OUTPUT);
	mainServo.attach(11);
	Serial.begin(9600);
	
	setPassword();
}

void loop() {
	if(!locked) {
		int switchVal = digitalRead(switchPin);
		if(switchVal == HIGH) {
			// If switch is engaged, power redLED and set
			// servo to "locked" position
			locked = true;
			digitalWrite(greenLED, LOW);
			digitalWrite(redLED, HIGH);
			mainServo.write(90);
			Serial.print("Servo: locked");
			delay(10);
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
 * User enters a valid length character sequence in the keypad. The program
 * waits until the user enters enough characters.
 *
 * @param leds An integer list of LED pins to be affected
 * @param arrLength An integer representing the length of the leds array
 * @param endHigh A boolean determining if LEDs end on HIGH or LOW
 **/
void userKeypadEntry(int leds[], int arrLength, bool endHigh) {
	char keyPress;
	for(int i = 0; i < PASSWORD_LEN; i++) {
		keyPress = NO_KEY;
		while(keyPress == NO_KEY) {
			keyPress = mainKeypad.getKey();
		}
		userEntry[i] = keyPress;
		// LEDs indicate a key press was accepted
		flashLEDs(leds, arrLength, 1, FAST_BLINK, endHigh);
		Serial.print(keyPress);
	}
}

/**
 * Checks the keypad entry to the set password.
 *
 * @param entry A char array which holds the entry to be checked
 * @param password A char array that holds the saved password
 **/
bool checkKeyEntry(char entry[], char password[]) {
	for(int i = 0; i < PASSWORD_LEN; i++) {
		if(entry[i] != password[i]) {
			return false;
		}
	}
	
	return true;
}

/**
 * Flashes the specified leds a certain number of times.
 * 
 * @param leds An integer list of LED pins to be affected
 * @param arrLength An integer representing the length of the leds array
 * @param count An integer for number of LED blinks
 * @param blinkSpeed An integer representing blink delay
 * @param endHigh A boolean determining if LEDs end on HIGH or LOW
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

/**
 * Enables the user to enter a PASSWORD_LEN char password in the keypad.
 * After the password is set, the servo moves to the unlocked position.
 **/
void setPassword() {
	// All LEDs will be activated and respond to key presses to highlight
	// the fact the password is being reset
	digitalWrite(greenLED, HIGH);
	digitalWrite(yellowLED, HIGH);
	digitalWrite(redLED, HIGH);
	Serial.print("Waiting for 4 key password entry: ");
	
	int leds[3] = {greenLED, yellowLED, redLED};
	userKeypadEntry(leds, 3, true);
	for(int i = 0; i < PASSWORD_LEN; i++) {
		password[i] = userEntry[i];
	}
	
	// Flash all LEDs to indicate password initialization
	flashLEDs(leds, 3, 2, MED_BLINK, false);
	Serial.println("\nPassword set");
	
	digitalWrite(greenLED, HIGH);
	mainServo.write(0);
	Serial.println("Servo: unlocked");
}