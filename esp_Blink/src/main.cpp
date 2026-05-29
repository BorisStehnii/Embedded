#include <Arduino.h>

#define LED_PIN_1 42
#define LED_PIN_2 2
#define BUTTON_PIN 0
#define BUTTON_EXTERNAL 40

int blinkDelay = 1000;

void setup() {
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_EXTERNAL, INPUT);   // зовнішній pull-up резистор
  Serial0.begin(115200);

  Serial0.println("System started");
  Serial0.println("BOOT button: INPUT_PULLUP");
  Serial0.println("External button: INPUT with external PULL-UP");
  Serial0.print("Initial blinkDelay = ");
  Serial0.println(blinkDelay);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  int buttonExternalState = digitalRead(BUTTON_EXTERNAL);

  Serial0.print("BOOT = ");
  Serial0.print(buttonState);
  Serial0.print(" | EXT = ");
  Serial0.print(buttonExternalState);
  Serial0.print(" | ");

  if (buttonState == LOW && buttonExternalState == HIGH) {
    blinkDelay = 1000;
    Serial0.println("Mode: SLOW (only BOOT pressed)");
    delay(200);
  }
  else if (buttonState == HIGH && buttonExternalState == LOW) {
    blinkDelay = 300;
    Serial0.println("Mode: FAST (only external button pressed)");
    delay(200);
  }
  else if (buttonState == LOW && buttonExternalState == LOW) {
    blinkDelay = 100;
    Serial0.println("Mode: VERY FAST (both buttons pressed)");
    delay(200);
  }
  else {
    Serial0.println("Mode: no buttons pressed, keeping previous delay");
  }

  Serial0.print("blinkDelay = ");
  Serial0.println(blinkDelay);

  digitalWrite(LED_PIN_1, HIGH);
  digitalWrite(LED_PIN_2, LOW);
  Serial0.println("LED1 ON, LED2 OFF");
  delay(blinkDelay);

  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, HIGH);
  Serial0.println("LED1 OFF, LED2 ON");
  delay(blinkDelay);

  Serial0.println("--------------------");
}

