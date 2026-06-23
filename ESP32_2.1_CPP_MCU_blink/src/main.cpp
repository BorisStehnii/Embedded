// блінк світодода з кнопкою на ESP32 , перемикання на швидке миготіння після натискання кнопки

#include <Arduino.h>

static constexpr uint8_t LED_PIN = 2;
static constexpr uint8_t BUTTON_PIN = 4;



// Стан світлодіода
enum class LedState
{
  Off, On
};

// Налаштування програми
class Config
{
public:
    static constexpr unsigned long BLINK_TIME = 500;
    static constexpr unsigned long FAST_BLINK_TIME = 150;

    // Кількість швидких блимань після натискання
    static const uint8_t SHORT_PRESS_BLINK_COUNT = 3;

    static constexpr unsigned long DEBOUNCE_TIME = 50;
};

// Клас для керування світлодіодом
class Led
{
public:
    void init()
    {
        pinMode(LED_PIN, OUTPUT);
        set(LedState::Off);
    }

    void set(LedState state)
    {
        if (state == LedState::On)
        {
            digitalWrite(LED_PIN, HIGH);
        }
        else
        {
            digitalWrite(LED_PIN, LOW);
        }
    }
};

Led led;


LedState ledState = LedState::Off;


void setup()
{
    led.init();

    // Кнопка підключена між GPIO4 і GND
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{

    static unsigned long lastBounceTime = 0;
    static bool lastButtonState = HIGH;

    static unsigned long lastLED = 0; // Таймер для керування миготінням
    static uint8_t togglesLeft = 0; // лічильник блимань

    int now = millis(); // визначаємо поточний час від початку роботи програми
    bool ButtonState = digitalRead(BUTTON_PIN); // зчитуємо стан кнопки

    // Перевірка натискання кнопки та дебаунс
    if (lastButtonState == HIGH && ButtonState == LOW) {
        if (now - lastBounceTime >= Config::DEBOUNCE_TIME) {
            lastBounceTime = now;

            togglesLeft = Config::SHORT_PRESS_BLINK_COUNT * 2; // 1 блимання це 2 стани леду (вкл/викл)

            ledState = LedState::Off;
            led.set(ledState);
        }
    }
    lastButtonState = ButtonState;

   
    unsigned long INTERVAL = Config::BLINK_TIME;

    // якщо лічильник більше нуля, то встановлюємо швидке миготіння
    if (togglesLeft > 0)
    {
        INTERVAL = Config::FAST_BLINK_TIME;
    }

    if (now - lastLED >= INTERVAL) {
        lastLED = now;

        if (ledState == LedState::Off)
        {
            ledState = LedState::On;
        }
        else
        {
            ledState = LedState::Off;
        }
        led.set(ledState);
        // digitalWrite(LED, ledState_1 ? HIGH : LOW);
        if (togglesLeft > 0)
        {
            togglesLeft--;
        }
    }
}