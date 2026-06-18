#include <Arduino.h>
#include <cmath>

constexpr uint8_t PHOTORESISTOR_PIN = 4; // Пін, до якого підключений фоторезистор

// Роздільна здатність АЦП — 12 біт
constexpr uint16_t ADC_MAX_VALUE = 4095;

// Напруга живлення подільника
constexpr float SUPPLY_VOLTAGE = 3.3f;

void setup()
{
    Serial.begin(115200);

    analogReadResolution(12);  // Роздільна здатність АЦП 12 біт

    analogSetPinAttenuation(PHOTORESISTOR_PIN, ADC_11db);

    delay(10000); // Затримка для запуску монітора серійного порту після перезавантаження плати

    Serial.println();
    Serial.println("RAW\tU_calc  \tU_measured  \tError \tError_%");
    Serial.println("-------------------------------------------------------------");
}

void loop()
{
    // RAW значення АЦП
    uint16_t RAWValue = analogRead(PHOTORESISTOR_PIN);
  
    // Обчислення напруги за формулою: U = (RAW / ADC_MAX_VALUE) * SUPPLY_VOLTAGE
    float CalcVoltage = static_cast<float>(RAWValue) * SUPPLY_VOLTAGE / static_cast<float>(ADC_MAX_VALUE);

    // Зчитування напруги в мілівольтах
    uint32_t ReadMillivolt = analogReadMilliVolts(PHOTORESISTOR_PIN);

    // Переведення мілівольтів у вольти
    float Voltage = static_cast<float>(ReadMillivolt) / 1000.0f;

    // Похибка у вольтах
    float Error = fabsf(CalcVoltage - Voltage);

    // Похибка у відсотках
    float Error_procent = 0.0f;

    if (Voltage > 0.0f)
    {
        Error_procent = Error / Voltage * 100.0f;
    }

    Serial.printf(
        "%u\t%.3f\t\t%.3f\t\t%.3f\t\t%.2f%%\n", 
        RAWValue,
        CalcVoltage,
        Voltage,
        Error,
        Error_procent
    );

    // Зчитування кожні 100мс
    delay(100);
}