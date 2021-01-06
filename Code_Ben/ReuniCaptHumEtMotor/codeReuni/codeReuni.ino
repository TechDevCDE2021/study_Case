#include "DHT.h"   // Librairie des capteurs DHT
#include <Wire.h>
#include <Adafruit_MotorShield.h>
// Cree un objet motor avec les I2C address defautl
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


#define DHTPIN 2    // PIN Branchement du cpateur hum / temp
#define DHTTYPE DHT22       // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

const int TourComplet = 180;
const int TroisQuartTour = 135;
const int MoitierTour = 90;
const int UnQuartTour = 45;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  dht.begin(); // Demarre capteur humidity               
  AFMS.begin();  // Cree avec par default la frequence 1.6KHz 
  myMotor->setSpeed(10);  // Donne la speedBase du moteur 10 rpm   
}

void loop() {
  // captHT (); // Capteur humidity / température
  // oneStep(); // Motor 
  actifMotor(); // ActifMotor selon les données envoyé par captHT
}

void unQuart() { // fonction pour faire un quart de tour
  myMotor->step(UnQuartTour, FORWARD, SINGLE);
  Serial.println("J'ai fais un quart de tour");
  delay(10000);

}

void moitier() { // fonction pour faire la moitier d'un tour 
  myMotor->step(MoitierTour, FORWARD, SINGLE);
  Serial.println("J'ai fais la moitier d'un tour ");
  delay(10000);

}

void troisQuart() { // fonction pour faire le trois quart d'un tour 
  myMotor->step(TroisQuartTour, FORWARD, SINGLE);
  Serial.println("J'ai fais le trois quart d'un tour ");
  delay(10000);

}

void tourComplet() { // fonction pour faire un tour complet 
  myMotor->step(TourComplet, FORWARD, SINGLE);
  Serial.println("J'ai fais un tour complet ");
  delay(10000);

}


void oneStep () { // void du motor
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

void captHT () {

  // Lecture du taux d'humidité
  float h = dht.readHumidity();
  // Lecture de la température en Celcius
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Echec de lecture !");
    return;
  }

  // Calcul la température ressentie. Il calcul est effectué à partir de la température en Fahrenheit
  // On fait la conversion en Celcius dans la foulée
  float hi = dht.computeHeatIndex(h);
  

  Serial.print("Humidite: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Temperature ressentie: ");
  Serial.print(dht.convertFtoC(hi));
  Serial.println(" *C");
  
}

void actifMotor () {
  // Lecture du taux d'humidité
  float h = dht.readHumidity();
  // Lecture de la température en Celcius
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Echec de lecture !");
    return;
  }
  if (h >= 50 && h <= 69 ) {
    unQuart();
   
  } else if (h >= 70 && h <= 84) {
    moitier();

  } else if (h > 85 && h <= 90) {
    troisQuart();

  } else if (h >= 91)  {
    tourComplet();
 
  }
  
  // Calcul la température ressentie. Il calcul est effectué à partir de la température en Fahrenheit
  // On fait la conversion en Celcius dans la foulée
  float hi = dht.computeHeatIndex(h);
  

  Serial.print("Humidite: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Temperature ressentie: ");
  Serial.print(dht.convertFtoC(hi));
  Serial.println(" *C");
  
}
