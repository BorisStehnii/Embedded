#include <Arduino.h>

#define BUTTON_PIN 2
#define DEBOUNCE_TIME 50
#define POLL_INTERVAL 5

int counter = 0;

unsigned long lastPollTime = 0;
unsigned long debounceStart = 0;

// машина станів
enum class ButtonState 
{
  Released,
  DebouncePress,
  Pressed,
  DebounceRelease
};
bool buttonPressed = false;

ButtonState buttonState = ButtonState::Released;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);

  Serial.begin(115200);
}

void loop()
{
  unsigned long now = millis();

  if (now - lastPollTime >= POLL_INTERVAL)
  {
    lastPollTime = now;

    buttonPressed = digitalRead(BUTTON_PIN) == LOW;
  }

  switch (buttonState)
  {
    case ButtonState::Released:
      // Кнопка була відпущена, але ми побачили LOW
      if (buttonPressed)
      {
        debounceStart = now;
        buttonState = ButtonState::DebouncePress;
      }
      break;


    case ButtonState::DebouncePress:
      // Чекаємо 50 мс і перевіряємо, чи кнопка досі натиснута
      if (now - debounceStart >= DEBOUNCE_TIME)
      {
        if (buttonPressed)
        {
          counter++;
          Serial.printf("Counter %d\n", counter);

          buttonState = ButtonState::Pressed;
        }
        else
        {
          // Це був брязкіт, повертаємось у стан відпущеної кнопки
          buttonState = ButtonState::Released;
        }
      }
      break;

    case ButtonState::Pressed:
      // Кнопка натиснута і вже зарахована, Тепер чекаємо початку відпускання
      if (!buttonPressed)
      {
        debounceStart = now;
        buttonState = ButtonState::DebounceRelease;
      }
      break;


    case ButtonState::DebounceRelease:
      // Чекаємо 50 мс і перевіряємо, чи кнопка досі відпущена
      if (now - debounceStart >= DEBOUNCE_TIME)
      {
        if (!buttonPressed)
        {
          buttonState = ButtonState::Released;
        }
        else
        {
          // Це був брязкіт при відпусканні, кнопка все ще натиснута
          buttonState = ButtonState::Pressed;
        }
      }
      break;
  }
}

////////////////////////////////////////////////////////////////////////////
// домашне 2.4 завдання 3
// #define BUTTON_PIN 2
// #define DEBOUNCE_TIME 50


// int counter = 0;

// volatile bool change_flag = false;
// volatile bool press_handled_flag = false; // флаг на запуск обробки натискання
// volatile bool debounce_flag = false; // флаг на запуск дебоунс
// volatile bool release_flag = false; // флаг на відпускання


// unsigned long debounce_Start = 0; // момент натискання кнопки
// unsigned long release_Start = 0; //момент відпускання



// void IRAM_ATTR ISR_button(){
//   change_flag = true;
// }

// void setup() {
//   pinMode(BUTTON_PIN, INPUT); 
//   Serial.begin(115200); 
//   attachInterrupt(BUTTON_PIN, ISR_button, FALLING); // апаратне переривання
// }

// void loop() {
  
//   unsigned long now = millis(); // зчитуємо ниниішній час

//   // перевіряємо флаг від апаратного переривання
//   if (change_flag){
    
//     change_flag = false;

//     if (!press_handled_flag && !debounce_flag){
//       debounce_Start = now;
//       debounce_flag = true;
//     }
//   }

//   // перевірка реального стану за 50мс
//   if (debounce_flag && now - debounce_Start >= DEBOUNCE_TIME) {
//       debounce_flag = false;

//       if (digitalRead(BUTTON_PIN) == LOW){

//         counter++;
//         press_handled_flag = true;

//         Serial.printf(" Counter %d \n", counter);
//       }
//   }
//   // очікуване відпускання. контролюємо відпускання перед очікуванням нового натискання. 
//   if (press_handled_flag){

//     if (digitalRead(BUTTON_PIN) == HIGH){

//       if (!release_flag){
//         release_Start = now;
//         release_flag = true;
//       }

//       // кнопка відпущенна стабільно 50мс
//       if (now - release_Start >= DEBOUNCE_TIME){
//         press_handled_flag = false;
//         release_flag = false;
//       }

//     }
//     else{
//       // це був брязкіт
//       release_flag = false;
//     }

//   }
// }


////////////////////////////////////////////////////////////////////////////
// домашне 2.4 завдання 2
// #define BUTTON_PIN 2


// int counter = 0;
// volatile bool change_flag = false;

// unsigned long last = 0;

// #define DEBOUNCE_TIME 50

// void IRAM_ATTR ISR_button(){
//   change_flag = true;
// }

// void setup() {
//   pinMode(BUTTON_PIN, INPUT);
//   Serial.begin(115200);
//   attachInterrupt(BUTTON_PIN, ISR_button, FALLING);
// }

// void loop() {
//   if (change_flag){
    
//     change_flag = false;

    
//     unsigned long now = millis();

//     if (now - last >= DEBOUNCE_TIME) {
//       last = now;
//       counter++;
//       Serial.printf(" Counter %d \n", counter);
//     }
//   }
// }




////////////////////////////////////////////////////////////////////////////
// домашне 2.4 завдання 1
// #define BUTTON_PIN 2


// volatile int counter = 0;
// volatile bool change_flag = false;


// void IRAM_ATTR INCREMENT_counter(){
//   counter++;
//   change_flag = true;
// }

// void setup() {
//   pinMode(BUTTON_PIN, INPUT);
//   Serial.begin(115200);
//   attachInterrupt(BUTTON_PIN, INCREMENT_counter, FALLING);
// }

// void loop() {
//   if (change_flag){
//     Serial.printf(" Counter %d \n", counter);
//     change_flag = false;
//   }
// }
