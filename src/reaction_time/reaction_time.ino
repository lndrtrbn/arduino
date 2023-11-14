#include <ezButton.h>

const int PIN_LED = 8;
const int PIN_BTN = 7;

const int DEBOUNCE = 50;

ezButton button(PIN_BTN);
int ledState = 0;

int myDelay = 0;
unsigned long sinceDelay = 0;
unsigned long now = 0;

void setup() {
  button.setDebounceTime(DEBOUNCE);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  button.loop();

  now = millis();
  if (myDelay == 0) {
    myDelay = random(500, 2000);
    sinceDelay = now;
  }

  if (ledState == 0 && now - sinceDelay > myDelay) {
    ledState = 1;
    digitalWrite(PIN_LED,  HIGH);
  }

  if(button.isPressed()) {
    ledState = 0;
    myDelay = 0;
    digitalWrite(PIN_LED, LOW);
  }

  delay(10);
}