#include <Arduino.h>

#define BUTTON_PIN 15

volatile int16_t counter_button = 0;
volatile bool button_event = false;

void IRAM_ATTR reaction_button() {
  counter_button++;
  button_event = true;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  delay(1000);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), reaction_button, FALLING);
}

void loop() {
  if (button_event) {
    button_event = false;

    Serial.print("Button pressed! Count: ");
    Serial.println(counter_button);

    delay(50);
  }
}