#include <Arduino.h>

#define BUTTON_PIN 2
#define DEBOUNCE_TIME 50


int counter = 0;

volatile bool change_flag = false;
volatile bool press_handling_flag = false; // флаг на запуск обробки натискання
volatile bool debounce_flag = false; // флаг на запуск дебоунс
volatile bool tempering_flag = false; // флаг на відпускання



unsigned long last = 0;
unsigned long debounce_Start = 0; // момент натискання кнопки
unsigned long tempering_Start = 0; //момент відпускання



void IRAM_ATTR ISR_button(){
  change_flag = true;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT); 
  Serial.begin(115200); 
  attachInterrupt(BUTTON_PIN, ISR_button, FALLING); // апаратне переривання
}

void loop() {
  
  unsigned long now = millis(); // зчитуємо ниниішній час

  // перевіряємо флаг від апаратного переривання
  if (change_flag){
    
    change_flag = false;

    if (!press_handling_flag && !debounce_flag){
      debounce_Start = now;
      debounce_flag = true;
    }
  }

  // перевірка реального стану за 50мс
  if (debounce_flag && now - debounce_Start >= DEBOUNCE_TIME) {
      debounce_flag = false;

      if (digitalRead(BUTTON_PIN) == LOW){

        counter++;
        press_handling_flag = true;

        Serial.printf(" Counter %d \n", counter);
      }
  }
  // очікуване відпускання 
  if (press_handling_flag){

    if (digitalRead(BUTTON_PIN) == HIGH){

      if (!tempering_flag){
        tempering_Start = now;
        tempering_flag = true;
      }

      // кнопка відпущенна стабільно 50мс
      if (now - tempering_Start >= DEBOUNCE_TIME){
        press_handling_flag = false;
        tempering_flag = false;
      }

    }
    else{
      // це був брязкіт
      tempering_flag = false;
    }

  }
}


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
