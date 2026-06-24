#include <Arduino.h>

#define BUTTON_PIN 2

volatile int counter = 0;
volatile bool change_flag = false;

void INCREMENT_counter(){
  counter++;
  change_flag = true;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
  attachInterrupt(BUTTON_PIN, INCREMENT_counter, FALLING);
}

void loop() {
  if (change_flag){
    Serial.printf(" Counter %d \n", counter);
    change_flag = false;
  }
}
