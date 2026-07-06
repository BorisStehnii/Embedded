#include <Arduino.h>

#define RESISTOR_PIN 4
#define TIMER 500

unsigned long last = 0;
unsigned long RAW = 0;

float U_manual = 0;
float U_cali = 0;
float Error = 0;


void setup() {
  Serial.begin(115200);

  pinMode(RESISTOR_PIN, INPUT);

  analogReadResolution(12);

  analogSetPinAttenuation(RESISTOR_PIN, ADC_11db);

  delay(3000); // затримка для запуску монітору

  Serial.println("RAW\tU_manual(mV)\tU_cali(mV)\tError(%)");
}

void loop() {

  unsigned long now = millis();
  if (now - last >= TIMER){
    last = now;
    RAW = analogRead(RESISTOR_PIN); // зчитування значення АЦП з піна

    U_manual = RAW * 3300.0 / 4095.0; // значення за формулою

    U_cali = analogReadMilliVolts(RESISTOR_PIN); //каліброване значення

    if(U_cali > 0){

      Error = abs(U_manual - U_cali)/U_cali * 100.0;
    }
    else{
      Error = 0;
    }
    Serial.printf("%d\t%.2f\t\t%.2f\t\t%.2f\n", RAW, U_manual, U_cali, Error);

  }

}
