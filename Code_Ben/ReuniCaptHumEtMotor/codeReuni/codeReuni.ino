#include "DHT.h"   // Librairie des capteurs DHT
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#define DHTPIN 5    // PIN Branchement du capteur hum / temp
#define DHTTYPE DHT22       // DHT 22  (AM2302)

// Cree un objet motor avec les I2C address defautl
Adafruit_MotorShield AFMS = Adafruit_MotorShield();  

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


DHT dht(DHTPIN, DHTTYPE);

const int MAX_FERME = 0; // TODO  : il faut récupérer la position moteur pour qu'il se ferme jusqu'à un certain point.
const int MAX_ROTATION = 88; // TODO : il faut récupérer la position moteur MAX pour qu'il s'ouvre jusqu'à un certain point 
const int TROIS_QUART = 67;
const int MOITIE_TOUR = 45;
const int QUART_TOUR = 22 ;

int voletPos = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Start // ");
  Serial.println("Le volet est en position : " + String(voletPos));
  dht.begin(); // Demarre capteur humidity               
  AFMS.begin();  // Cree avec par default la frequence 1.6KHz 
  myMotor->setSpeed(10);  // Donne la speedBase du moteur 10 rpm   
}

void loop() {
  // captHT (); // Capteur humidity / température
  // oneStep(); // Motor 
  actifMotor(); // ActifMotor selon les données envoyé par captHT
}

void openRotate(int degree) { // Fonction pour ouvrir le volet 
  myMotor->step(degree, FORWARD, SINGLE);
  voletPos += degree;
  if (degree == QUART_TOUR) {
    Serial.println("J'ai fais un quart de tour");
  } else if (degree == MOITIE_TOUR) {
    Serial.println("J'ai fais la moitier d'un tour ");
  } else if (degree == TROIS_QUART) {
    Serial.println("J'ai fais le trois quart d'un tour ");
  } else {
    Serial.println("J'ai fais un tour complet");
  }
  Serial.println("Mon volet est à la position :" + String(voletPos));
  delay(15000);
}

void closeRotate(int degree) { // Fonction pour fermer le volet 
  myMotor->step(QUART_TOUR, BACKWARD, SINGLE);
  voletPos -= degree;
  Serial.println("Mon volet est à la position :" + String(voletPos));
  delay(15000);
}

void actifMotor () { // FONCTION PRINCIPAL 
  // Lecture du taux d'humidité
  float h = dht.readHumidity();
  // Lecture de la température en Celcius
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Echec de lecture !");
    return;
  }
  if (voletPos >= MAX_ROTATION && h >= 40 ) {
    Serial.println("Le volet ne peut pas être plus ouvert");
  } else if  (voletPos >= MAX_ROTATION && h <= 40) {
      if (h <= 30 ) {
        closeRotate(MAX_FERME);    
      } else { 
        closeRotate(QUART_TOUR); 
      }
        } else {
          if (h >= 50 && h <= 69 ) {
            openRotate(QUART_TOUR); // 45
            
          } else if (h >= 70 && h <= 84) {
            openRotate(MOITIE_TOUR); // 90
          
          } else if (h > 85 && h <= 90) {
            openRotate(TROIS_QUART); // 135
            
          } else if (h >= 91)  {
            openRotate(MAX_ROTATION); // 180
            
          }
    }
  
  // Calcul la température ressentie. Il calcul est effectué à partir de la température en Fahrenheit
  // On fait la conversion en Celcius dans la foulée
  float hi = dht.computeHeatIndex(h);
  

  Serial.print("Humidite: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" C ");
  Serial.print("Temperature ressentie: ");
  Serial.print(dht.convertFtoC(hi));
  Serial.println(" C");
  
}
