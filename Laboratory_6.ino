#include <Arduino.h>

const uint8_t ENA = 23;
const uint8_t IN1 = 18;
const uint8_t IN2 = 19;
const uint8_t BTN = 27;

bool lastPressed = false;

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);   

  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.println("Ready. Hold the button to run the motor.");
}

void loop() {
  const bool pressed = (digitalRead(BTN) == LOW);

  if (pressed != lastPressed) {
    lastPressed = pressed;

    if (pressed) {
      digitalWrite(IN1, HIGH);   
      digitalWrite(IN2, LOW);
      digitalWrite(ENA, HIGH);   
      Serial.println("Motor ON");
    } else {
      digitalWrite(ENA, LOW);    
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      Serial.println("Motor OFF (coast)");
    }
  }

  delay(20);   
}
