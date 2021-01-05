#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11
#include <AFMotor.h>

unsigned int speed = 5000;    // 2000make a modification for your test
AF_Stepper motor(64, 2);

void setup() {
pinMode(STEPPER_PIN_1, OUTPUT);
pinMode(STEPPER_PIN_2, OUTPUT);
pinMode(STEPPER_PIN_3, OUTPUT);
pinMode(STEPPER_PIN_4, OUTPUT);
motor.setSpeed(speed);

Serial.begin(9600);

}

void loop() {
  digitalWrite(STEPPER_PIN_1, HIGH);
  Serial.println("PIN 1 ACTIVE");
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  Serial.println("PIN 1 DESACTIVE");
  Serial.println("PIN 2 ACTIVE");
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  Serial.println("PIN 1,2 DESACTIVE");
  Serial.println("PIN 3 ACTIVE");
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  Serial.println("PIN 1,2,3 DESACTIVE");
  Serial.println("PIN 4 ACTIVE");
  Serial.println("J'ai fais un tour frere");
  
}
