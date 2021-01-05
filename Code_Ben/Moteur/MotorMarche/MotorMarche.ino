#include <Wire.h>
#include <Adafruit_MotorShield.h>
// Cree un objet motor avec les I2C address defautl
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

const int TourComplet = 180;
const int UnQuartTour = 45;
const int DoubleTour = 90;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
                  
  AFMS.begin();  // Cree avec par default la frequence 1.6KHz
  
  myMotor->setSpeed(10);  // 10 rpm   
}

void loop() {
 
  oneStep();
//  myMotor->step(360, BACKWARD, SINGLE); 

//  Serial.println("Double coil steps");
//  myMotor->step(100, FORWARD, DOUBLE); 
//  myMotor->step(100, BACKWARD, DOUBLE);
//  
//  Serial.println("Interleave coil steps");
//  myMotor->step(100, FORWARD, INTERLEAVE); 
//  myMotor->step(100, BACKWARD, INTERLEAVE); 
//  
//  Serial.println("Microstep steps");
//  myMotor->step(50, FORWARD, MICROSTEP); 
//  myMotor->step(50, BACKWARD, MICROSTEP);
}

void oneStep () {
  myMotor->step(UnQuartTour, FORWARD, SINGLE);
  Serial.println("J'ai fais un quart de tour");
  myMotor->step(UnQuartTour, FORWARD, SINGLE);
  Serial.println("J'ai fais un quart de tour");
  myMotor->step(UnQuartTour, FORWARD, SINGLE);
  Serial.println("J'ai fais un quart de tour");
  myMotor->step(UnQuartTour, FORWARD, SINGLE);
  Serial.println("J'ai fais un quart de tour");
  myMotor->step(UnQuartTour, FORWARD, SINGLE);
  Serial.println("J'ai fais un tour complet.");
}
