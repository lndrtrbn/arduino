unsigned const PIN_LED = 8;
unsigned const PIN_BTN = 7;

unsigned int bState = 0;

void setup() {
  pinMode(PIN_BTN, INPUT);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  if(digitalRead(PIN_BTN) == 1) { 
  	digitalWrite(PIN_LED, HIGH);
    bState += 1;
    if(bState % 2 == 0){
      digitalWrite(PIN_LED, LOW);
      bState = 0;
    }
  }
  delay(10);
}