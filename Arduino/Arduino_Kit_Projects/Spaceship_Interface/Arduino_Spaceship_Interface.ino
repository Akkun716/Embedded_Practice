/**
 * The program powers 3 LED lights (one green, two red) with a push-button.
 * Unpressed: Green LED stays on and red LEDs are off.
 * Pressed: green LED turns off and two red LEDs alternate.
 */

// Switchstate: Pushbutton switch used
int switchstate = 0;
void setup() {
  // Pin 3: green LED; Pins 4 & 5: red LEDs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  // Pin 2: push-button
  pinMode(2, INPUT);
}

void loop() {
  switchstate = digitalRead(2);

  // Unpressed
  if(switchstate == LOW) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  
  // Pressed
  } else {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delay(250);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    delay(250);
  }
}
