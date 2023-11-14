#include <ezButton.h>

const int PIN_LED = 8;
const int PIN_BTN = 7;

const int DEBOUNCE = 50;

ezButton button(PIN_BTN);
int ledState = 0;

void setup() {
  button.setDebounceTime(DEBOUNCE);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  button.loop();

  if(button.isPressed()) {
    ledState = (ledState + 1) % 2;
    digitalWrite(PIN_LED, ledState == 1 ? HIGH : LOW);
  }

  delay(10);
}