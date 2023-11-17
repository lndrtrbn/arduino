#include <Wire.h>
#include <ezButton.h>
#include "rgb_lcd.h"
 
// =======================================
// Batak Mini - by Landry Trebon - 11/2023
// =======================================

// CONSTANTS =============================

const int NB_BUTTONS = 4;

const int PIN_LED_START = 2;
const int PIN_BTN_START = 8;

const int BTN_SWITCH = PIN_BTN_START;
const int BTN_VALIDATE = PIN_BTN_START + 1;

const int GAME_DURATION = 20;
const int NEW_LED_INTERVAL = 300;

const int NB_MODES = 6;
const int MODES[NB_MODES] = { -1, 1000, 800, 600, 400, 200 };
const String MODE_NAMES[NB_MODES] = {
  "Peaceful",
  "Hard 1s",
  "Hard 0.8s",
  "Hard 0.6s",
  "Hard 0.4s",
  "Hard 0.2s"
};

const int PIN_LED_1 = 2;
const int PIN_BTN_1 = 8;

const int PIN_LED_2 = 3;
const int PIN_BTN_2 = 9;

const int PIN_LED_3 = 4;
const int PIN_BTN_3 = 10;

const int PIN_LED_4 = 5;
const int PIN_BTN_4 = 11;

const int DEBOUNCE = 50;

// VARIABLES =============================

ezButton button1(PIN_BTN_1);
ezButton button2(PIN_BTN_2);
ezButton button3(PIN_BTN_3);
ezButton button4(PIN_BTN_4);

int mode = -1;
rgb_lcd lcd;

// SETUP =================================

void prepare() { 
  randomSeed(analogRead(0));

  button1.setDebounceTime(DEBOUNCE);
  pinMode(PIN_LED_1, OUTPUT);
  digitalWrite(PIN_LED_1, HIGH);

  button2.setDebounceTime(DEBOUNCE);
  pinMode(PIN_LED_2, OUTPUT);
  digitalWrite(PIN_LED_2, HIGH);

  button3.setDebounceTime(DEBOUNCE);
  pinMode(PIN_LED_3, OUTPUT);
  digitalWrite(PIN_LED_3, HIGH);

  button4.setDebounceTime(DEBOUNCE);
  pinMode(PIN_LED_4, OUTPUT);
  digitalWrite(PIN_LED_4, HIGH);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("== BATAK MINI ==");
}

void flashLed() {
  int i = 0;
  while (i < 3) {
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);
    digitalWrite(PIN_LED_3, LOW);
    digitalWrite(PIN_LED_4, LOW);
    delay(300);

    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
    digitalWrite(PIN_LED_3, HIGH);
    digitalWrite(PIN_LED_4, HIGH);
    delay(200);
    i++;
  }
}

void setup() {
  Serial.begin(9600);
  prepare();
  flashLed();
}

// LOOP ==================================

void gameMenu() {
  mode = -1;
  int selectedMode = 0;
  lcd.setCursor(0, 1);
  lcd.print("Mode:");

  while (1 == 1) {
    if (selectedMode != mode) {
      mode = selectedMode;
      lcd.setCursor(6, 1);
      lcd.print("         ");
      lcd.setCursor(6, 1);
      lcd.print(MODE_NAMES[mode]);
    }

    if (button2.isPressed()) {
      selectedMode = (mode + 1) % NB_MODES;
    }

    if (button1.isPressed()) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      break;
    }
  }
}

void startGame() {
  unsigned long now = 0;
  int timeRemaining = 0;
  int localTimeRemaining;

  int score = 0;
  int activeLed = 0;
  unsigned long lastLedOff = 0;

  flashLed();
  lcd.setCursor(0, 1);
  lcd.print("Score:");
  lcd.setCursor(6, 1);
  lcd.print(score);
  lcd.setCursor(15, 1);
  lcd.print("s");

  unsigned long gameStart = millis();

  while (1 == 1) {
    now = millis();
  
    localTimeRemaining = GAME_DURATION - ((now - gameStart) / 1000);
    if (localTimeRemaining != timeRemaining) {
      timeRemaining = localTimeRemaining;
      lcd.setCursor(13, 1);
      lcd.print("  ");
      lcd.setCursor(13, 1);
      lcd.print(timeRemaining);
    }
    
    if (activeLed == 0 && (now - lastLedOff) > NEW_LED_INTERVAL) {
      activeLed = random(PIN_LED_START, PIN_LED_START+NB_BUTTONS);
      digitalWrite(activeLed, LOW);
    }
    
    if (
      (button1.isPressed() && activeLed == PIN_LED_1) ||
      (button2.isPressed() && activeLed == PIN_LED_2) ||
      (button3.isPressed() && activeLed == PIN_LED_3) ||
      (button4.isPressed() && activeLed == PIN_LED_4) ||
    ) {
      digitalWrite(activeLed, HIGH);
      activeLed = 0;
      lcd.setCursor(6, 1);
      lcd.print(++score);
      lastLedOff = now;
      break;
    }

    if (MODES[mode] > 0 && now - lastLedOff > (MODES[mode] + NEW_LED_INTERVAL)) {
      digitalWrite(activeLed, HIGH);
      activeLed = 0;
      lastLedOff = now;
    }

    if (timeRemaining <= 0) {
      flashLed();
      break;
    }
  }
}

void endGame() {
  lcd.setCursor(10, 1);
  lcd.print("Again?");

  while (1 == 1) {
    if (button1.isPressed()) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      break;
    }
  }
}

void loop() {
  button1.loop();
  button2.loop();
  button3.loop();
  button4.loop();

  gameMenu();
  startGame();
  endGame();
}