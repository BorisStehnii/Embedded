#include <Arduino.h>

#define RELAY_PIN 4
#define POSITION_RELAY_PIN 5
#define BUTTON_PIN 2

#define DEBOUNCE_TIME 150
#define RELAY_DEBONCE_TIME 20

bool STATUS_RELAY = false;



// Увімкнення реле через транзистор
void relay_On()
{
  digitalWrite(RELAY_PIN, HIGH);
}

// Вимкнення реле
void relay_Off()
{
  digitalWrite(RELAY_PIN, LOW);
}

void relay_SetState(bool Status)
{

  if (Status) {
    relay_On();
  }
  else {
    relay_Off();
  }

}

void setup() {

pinMode(BUTTON_PIN, INPUT_PULLUP);
pinMode(RELAY_PIN, OUTPUT);
pinMode(POSITION_RELAY_PIN, INPUT_PULLUP);
Serial.begin(115200);

relay_SetState(STATUS_RELAY); // Початковий стан реле - вимкнено

}


void loop() {

  unsigned long now = millis();

  static unsigned long time_command_change_state = 0;
  
  static bool command_relay = false;

  static unsigned long lastPressTime = 0;
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);


  if (lastButtonState == HIGH && buttonState == LOW) {
    if (now - lastPressTime >= DEBOUNCE_TIME) {
      lastPressTime = now;

      STATUS_RELAY = !STATUS_RELAY; // змінюємо стан реле на протилежний
      relay_SetState(STATUS_RELAY); // керування реле відповідно до STATUS_RELAY
      time_command_change_state = now;
      command_relay = true;  // зміни в реле без команди не будуть враховані (якщо перебої в живленні)
    }
  }
  lastButtonState = buttonState;


  bool RelayState = digitalRead(POSITION_RELAY_PIN);
  static bool pastRelayState = RelayState; // попередня зміна стану
  static bool confermRelayState = RelayState; // перевірена зміна стану 
  static unsigned long pastRelayStateTime = 0; // попередній момент зміни стану реле
  static unsigned long relay_operation_time = 0; // час зміни стану реле

  // відслідковуємо зміну стану
  if (RelayState!=pastRelayState){

    pastRelayState = RelayState;
    pastRelayStateTime = now; // записуємо останню зміну положення реле
  }
  // перевіряємо на брязкіт. якщо пройшов час стабільного стану переходимо для підрахунку значення. 
  if ((now - pastRelayStateTime >= RELAY_DEBONCE_TIME) && (RelayState != confermRelayState)){
      
    confermRelayState = RelayState;
    if (command_relay){
      relay_operation_time = pastRelayStateTime - time_command_change_state;

      if (confermRelayState) {
        Serial.printf("Relay is OFF. Time since command: %lu ms\n", relay_operation_time);
      }
      else {
        Serial.printf("Relay is ON. Time since command: %lu ms\n", relay_operation_time);
      }
    }
  }

  


// Зайвий шматок для відстеження часу відкриття та закриття реле. бо неодразу зрозумів завдання....
  // static bool Initial = false;
  // static unsigned long last_time_RelayState = 0;
  // static bool pastRelayState = true;

  // if (!Initial) {
  //   Initial = true;
  //   pastRelayState = RelayState;
  //   last_time_RelayState = now;
  // }
  // else if (RelayState != pastRelayState) {
    
  //   unsigned long stateTime = now - last_time_RelayState;

  //   if(RelayState) {
  //     Serial.printf("Time Relay close: %lu ms\n", stateTime);
  //   }
  //   else {
  //     Serial.printf("Time Relay open: %lu ms\n", stateTime);
  //   }

  //   last_time_RelayState = now;
  //   pastRelayState = RelayState;
  // }
}

