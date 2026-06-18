#include <Arduino.h>

#define NTC_PIN 5
#define RELE_PIN 4
#define LED_RED 41
#define LED_BLUE 42

#define TEMP_THRESHOLD 2000


bool STATUS_RELAY = true;


void relay_On() {
  digitalWrite(RELE_PIN, HIGH);
}

void relay_Off() {
  digitalWrite(RELE_PIN, LOW);
}

void led_off()
{
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
}
void led_on()
{
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, LOW);
    delay(300);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, HIGH);
    delay(300);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELE_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(NTC_PIN, INPUT);

  analogReadResolution(12);

  relay_On();
  led_off();

  delay(10000); // Затримка для запуску монітора серійного порту після перезавантаження плати
  Serial.println("System initialized. Monitoring NTC sensor...");
  Serial.println("led_off, relay_on");

}

void loop() {
  int NTC_Value = analogRead(NTC_PIN);

  Serial.printf("NTC Value: %d\n", NTC_Value);

  if (NTC_Value >= TEMP_THRESHOLD)
    {
      if (STATUS_RELAY)
      {
        relay_Off();
        STATUS_RELAY = false;
        Serial.println("LED on, relay off");
        }

        led_on();
    }
  else {
    if (!STATUS_RELAY) 
    {
      // НОРМА: реле ввімкнено, LED вимкнені
      relay_On();
      led_off();

      STATUS_RELAY = !STATUS_RELAY;

      Serial.println("led_off, relay_on");
    }
    delay(1000); // Затримка для зменшення частоти зчитування та виводу в серійний монітор
  }

}