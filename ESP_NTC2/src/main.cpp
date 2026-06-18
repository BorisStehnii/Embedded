
#include <Arduino.h>

// Піни підключення
#define NTC_PIN 5
#define RELE_PIN 4
#define LED_RED 41
#define LED_BLUE 42

// Порогове значення АЦП для аварійного режиму
#define TEMP_THRESHOLD 2000

// Поточний стан реле 
bool STATUS_RELAY = true;

// Увімкнення реле через транзистор
void relay_On()
{
  digitalWrite(RELE_PIN, HIGH);
}

// Вимкнення реле
void relay_Off()
{
  digitalWrite(RELE_PIN, LOW);
}

// Вимкнення обох світлодіодів
void led_off()
{
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
}

// Почергове блимання світлодіодів
void led_on()
{
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, LOW);
  delay(300);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, HIGH);
  delay(300);
}

void setup()
{
  Serial.begin(115200);

  pinMode(RELE_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(NTC_PIN, INPUT);

  analogReadResolution(12);

  // Початковий стан системи
  relay_On();
  led_off();

  // Очікування запуску Serial Monitor
  delay(10000);

  Serial.println("System initialized. Monitoring NTC sensor...");
  Serial.println("LED off, relay on");
}

void loop()
{
  // Зчитування значення з NTC-датчика
  int NTC_Value = analogRead(NTC_PIN);

  Serial.printf("NTC Value: %d\n", NTC_Value);

  if (NTC_Value >= TEMP_THRESHOLD)
  {
    // Аварійний режим
    if (STATUS_RELAY)
    {
      relay_Off();
      STATUS_RELAY = false;

      Serial.println("LED on, relay off");
    }

    led_on();
  }
  else
  {
    // Повернення до штатного режиму
    if (!STATUS_RELAY)
    {
      relay_On();
      led_off();

      STATUS_RELAY = true;

      Serial.println("LED off, relay on");
    }

    delay(1000);
  }
}
