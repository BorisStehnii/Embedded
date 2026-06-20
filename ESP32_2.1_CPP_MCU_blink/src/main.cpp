#include <Arduino.h>

// Стан світлодіода
enum class LedState
{
  Off, On
};

// Налаштування програми
class Config
{
public:
    static constexpr uint8_t LED_PIN = 42;
    static constexpr uint8_t BUTTON_PIN = 40;

    static constexpr unsigned long BLINK_TIME = 500;
    static constexpr unsigned long FAST_BLINK_TIME = 150;
    static constexpr unsigned long DEBOUNCE_TIME = 50;

    // Кількість швидких блимань після натискання
    static const uint8_t SHORT_PRESS_BLINK_COUNT = 3;
};

// Простий клас світлодіода
class Led
{
public:
    void init()
    {
        pinMode(Config::LED_PIN, OUTPUT);
        set(LedState::Off);
    }

    void set(LedState state)
    {
        if (state == LedState::On)
        {
            digitalWrite(Config::LED_PIN, HIGH);
        }
        else
        {
            digitalWrite(Config::LED_PIN, LOW);
        }
    }
};

Led led;

// Єдина глобальна змінна стану LED
LedState ledState = LedState::Off;

void setup()
{
    led.init();

    // Кнопка підключена між GPIO40 і GND
    pinMode(Config::BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
    // static-змінні зберігають значення між викликами loop()
    static unsigned long previousTime = 0;
    static unsigned long lastButtonTime = 0;

    static uint8_t togglesLeft = 0;
    static bool previousButtonState = HIGH;

    const unsigned long currentTime = millis();
    const bool currentButtonState =
        digitalRead(Config::BUTTON_PIN);

    // Перевірка натискання кнопки
    if (previousButtonState == HIGH &&
        currentButtonState == LOW &&
        currentTime - lastButtonTime >= Config::DEBOUNCE_TIME)
    {
        lastButtonTime = currentTime;

        // Одне блимання — це ввімкнення і вимкнення
        togglesLeft =
            Config::SHORT_PRESS_BLINK_COUNT * 2;
    }

    previousButtonState = currentButtonState;

    // Під час швидкого блимання використовуємо менший інтервал
    unsigned long blinkInterval = Config::BLINK_TIME;

    if (togglesLeft > 0)
    {
        blinkInterval = Config::FAST_BLINK_TIME;
    }

    // Неблокуюче керування світлодіодом
    if (currentTime - previousTime >= blinkInterval)
    {
        previousTime = currentTime;

        if (ledState == LedState::Off)
        {
            ledState = LedState::On;
        }
        else
        {
            ledState = LedState::Off;
        }

        led.set(ledState);

        if (togglesLeft > 0)
        {
            togglesLeft--;
        }
    }
}