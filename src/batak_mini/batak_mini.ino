#include <Wire.h> 
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

// VARIABLES =============================

int mode = -1;
rgb_lcd lcd;

// SETUP =================================

void prepare() { 
  randomSeed(analogRead(0));

  int i = 0;
  while (i < NB_BUTTONS) {
    pinMode(PIN_BTN_START+i, INPUT);
  	pinMode(PIN_LED_START+i, OUTPUT);
    digitalWrite(PIN_LED_START+i, HIGH);
    i++;
  }
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("== BATAK MINI ==");
}

void flashLed() {
  int j = 0;
  while (j < 3) {
    int i = 0;
    while (i < NB_BUTTONS) {
      digitalWrite(PIN_LED_START+i, LOW);
      i++;
    }
    delay(300);
    i = 0;
    while (i < NB_BUTTONS) {
      digitalWrite(PIN_LED_START+i, HIGH);
      i++;
    }
    delay(200);
    j++;
  }
}

void setup() {
  Serial.begin(9600);
  prepare();
  flashLed();
  Serial.println("BTN_SWITCH");
  Serial.println(BTN_SWITCH);
  Serial.println("BTN_VALIDATE");
  Serial.println(BTN_VALIDATE);
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

    if (digitalRead(BTN_SWITCH) == HIGH) {
      Serial.println("SWITCH MODE");
      selectedMode = (mode + 1) % NB_MODES;
    }

    if (digitalRead(BTN_VALIDATE) == HIGH) {
      Serial.println("START GAME");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      break;
    }

    delay(10);
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
    
    int i = 0;
    while (i < NB_BUTTONS) {
      if (digitalRead(PIN_BTN_START + i) == HIGH && activeLed == PIN_LED_START + i) {
        digitalWrite(activeLed, HIGH);
        activeLed = 0;
        lcd.setCursor(6, 1);
        lcd.print(++score);
        lastLedOff = now;
        break;
      }
      i++;
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

    delay(10);
  }
}

void endGame() {
  lcd.setCursor(10, 1);
  lcd.print("Again?");

  while (1 == 1) {
    if (digitalRead(BTN_VALIDATE) == HIGH) {
      lcd.setCursor(0, 1);
      lcd.print("                ");
      break;
    }

    delay(10);
  }
}

void loop() {
  gameMenu();
  startGame();
  endGame();
}