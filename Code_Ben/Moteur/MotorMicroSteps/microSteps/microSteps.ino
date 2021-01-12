#include <Wire.h>
#include <Adafruit_MotorShield.h>
// Cree un objet motor avec les I2C address defautl
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

const int TourComplet = 180;
const int DoubleTour = 90;
const int UnQuartTour = 45;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
                  
  AFMS.begin();  // Cree avec par default la frequence 1.6KHz
  
  myMotor->setSpeed(10);  // 10 rpm   
}

void loop() {
 
  oneStep();
} 

void oneStep () {
  while (true) {
    myMotor->step(TourComplet, FORWARD, MICROSTEP);
  }
}
