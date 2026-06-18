#include <Arduino.h>

#define LED_PIN_1 42
#define LED_PIN_2 41
#define BUTTON_PIN 0
#define BUTTON_EXTERNAL 40

int blinkDelay = 1000;

void setup() {
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_EXTERNAL, INPUT);   // зовнішній pull-up резистор
}

void loop() {
  
  int buttonState = digitalRead(BUTTON_PIN);
  int buttonExternalState = digitalRead(BUTTON_EXTERNAL);


  if (buttonState == LOW && buttonExternalState == HIGH) {
    blinkDelay = 1000;
    delay(200);
  }
  else if (buttonState == HIGH && buttonExternalState == LOW) {
    blinkDelay = 300;
    delay(200);
  }
  else if (buttonState == LOW && buttonExternalState == LOW) {
    blinkDelay = 100;

    delay(200);
  }
  
  digitalWrite(LED_PIN_1, HIGH);
  digitalWrite(LED_PIN_2, LOW);
  delay(blinkDelay);

  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, HIGH);
  
  delay(blinkDelay);
}

