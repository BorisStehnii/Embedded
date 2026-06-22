#include <Arduino.h>

const int LED_1 = 41;
const int LED_2 = 42;
const int LED_3 = 2;

#define BUTTON_PIN 4

// Інтервали для миготіння
#define INTERVAL 500
#define PING_LED 250

#define DEBOUNCE_TIME 50

// Змінні стану світлодіодів
bool ledState_1 = false;
bool ledState_2 = false;
bool ledState_3 = false;


int lastLED_Direct[] = {0, 250, 500};
int lastLED_Convers[] = {500, 250, 0};
// int lastLED_3 = 500;

int lastBounceTime = 0;
bool lastButtonState = HIGH;  
bool blinkingMode = false;

// Функція для скидання стану світлодіодів та таймерів
void resetBlink(unsigned long now) {
  ledState_1 = false;
  ledState_2 = false;
  ledState_3 = false;

  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);

  if (blinkingMode) {
    lastLED_Direct[0] = now - INTERVAL;
    lastLED_Direct[1] = now - (INTERVAL - PING_LED);
    lastLED_Direct[2] = now;
  }
  else {
    lastLED_Convers[0] = now;
    lastLED_Convers[1] = now - (INTERVAL - PING_LED);
    lastLED_Convers[2] = now - INTERVAL;
  }
}

// Функція для керування миготінням світлодіодів пряме та зворотне
void setBlink_1(int now_1, bool State, int lastLED_direct[], int lastLED_convers[]) {
  // Пряме миготіння
  if (State){

    if (now_1 - lastLED_direct[0] >= INTERVAL) {
        lastLED_direct[0] = now_1;
        ledState_1 = !ledState_1;
        digitalWrite(LED_1, ledState_1 ? HIGH : LOW);
    }

    if (now_1 - lastLED_direct[1] >= INTERVAL) {
        lastLED_direct[1] = now_1;
        ledState_2 = !ledState_2;
        digitalWrite(LED_2, ledState_2 ? HIGH : LOW);
    }

    if (now_1 - lastLED_direct[2] >= INTERVAL) {
        lastLED_direct[2] = now_1;
        ledState_3 = !ledState_3;
        digitalWrite(LED_3, ledState_3 ? HIGH : LOW);
    }
  }
  // Зворотне миготіння
  else {

    if (now_1 - lastLED_convers[0] >= INTERVAL) {
        lastLED_convers[0] = now_1;
        ledState_1 = !ledState_1;
        digitalWrite(LED_1, ledState_1 ? HIGH : LOW);
    }

    if (now_1 - lastLED_convers[1] >= INTERVAL) {
        lastLED_convers[1] = now_1;
        ledState_2 = !ledState_2;
        digitalWrite(LED_2, ledState_2 ? HIGH : LOW);
    }

    if (now_1 - lastLED_convers[2] >= INTERVAL) {
        lastLED_convers[2] = now_1;
        ledState_3 = !ledState_3;
        digitalWrite(LED_3, ledState_3 ? HIGH : LOW);
    }
  }
}



void setup() {
pinMode(LED_1, OUTPUT);
pinMode(LED_2, OUTPUT);
pinMode(LED_3, OUTPUT);
pinMode(BUTTON_PIN, INPUT_PULLUP);
Serial.begin(115200);

}

void loop() {

  int now = millis();

  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    if (now - lastBounceTime >= DEBOUNCE_TIME) {
      lastBounceTime = now;
      // Реверс режиму миготіння
      blinkingMode = !blinkingMode;
      // Скидання стану світлодіодів та таймерів
      resetBlink(now);
    }
  }

  lastButtonState = buttonState;
  
  setBlink_1(now, blinkingMode, lastLED_Direct, lastLED_Convers);
  
} 
