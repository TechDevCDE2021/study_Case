#include <DHT.h>
#include <DHT_U.h>
#include <TheThingsNetwork.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// ##### NE PAS UTILSER LE PIN 2 AVEC LEONARDO POUR CONNEXION TTN  ##### //

#define CAPTVENT 7 // PIN Branchement du capteur de vent 
#define DHTPIN 5    // PIN Branchement du capteur hum / temp
#define DHTTYPE DHT22       // DHT 22  (AM2302)

#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868


const char *appEui = "70B3D57ED003AE78";
const char *appKey = "20E15BFC27CD9836F96A16D16C3FE6C0";
const int MAX_FERME = 0; // TODO  : il faut récupérer la position moteur pour qu'il se ferme jusqu'à un certain point.
const int MAX_ROTATION = 90; // TODO : il faut récupérer la position moteur MAX pour qu'il s'ouvre jusqu'à un certain point.
const int ROTATE = 45;  // Tourne le moteur

// Calcule du vent

int vitesse;
unsigned long tPer;
volatile unsigned long tActu, tPrec;

// ----------------------------------

// Savoir si le volet est ouvert ou fermé

boolean isOpen = true;
int voletPos = 0;

// ----------------------------------

// Cree un objet motor avec les I2C address defautl
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// TIPS : 200 STEPS = UN TOUR COMPLET DU MOTEUR
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


DHT dht(DHTPIN, DHTTYPE);
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

//   Lecture du taux d'humidité
//   unit32_t is a numeric type that guarantees 32 bits
uint32_t humidity;
// Lecture de la température en Celcius
uint32_t temperature;
// Lecture du vent
uint32_t wind;


void setup()
{
  pinMode(CAPTVENT, INPUT); // Capteur Vent
  attachInterrupt(digitalPinToInterrupt(CAPTVENT), inter, RISING); // Interrupt Capt.Vent
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);
  debugSerial.println("-- STATUS");
  ttn.showStatus();
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
  ttn.onMessage(message);
  dht.begin();
  AFMS.begin();  // Cree avec par default la frequence 1.6KHz
  myMotor->setSpeed(10);  // Donne la speedBase du moteur 10 rpm
}

void loop()
{
  humidity = dht.readHumidity(false) * 100;
  temperature = dht.readTemperature(false) * 100;
  sendHT(); // Données envoyé par le capteur
  delay(6000);
  if (wind > 10 && isOpen) { // Première condition, si il y a trop de vent, le volet se ferme.
    Serial.print("TROP DE VENT ON FERME ");
    closeRotate(ROTATE);
  }
//  Serial.print("Le vent est de : ");
//  Serial.print(wind);
//  Serial.println(" km/h");
  captWind();
}


void message (const uint8_t *payload, size_t size, port_t port) {
  int i;
  switch (payload[i]) {
    case 0:
    case 1:
      Serial.println("----------------------------");
      Serial.println("Mode Manuel");
      if ( payload[0] == 1 && !isOpen) {
        Serial.println(" OUVRE, mode Manuel");
        openRotate(ROTATE);
      } else if (payload[0] == 0 && isOpen) {
        Serial.println(" FERME, mode Manuel");
        closeRotate(ROTATE);
      } else {
        Serial.println("Je ne peux pas faire ce que tu veux ");
      }
      break;
    case 2:
      Serial.println("-------------------------------------");
      Serial.println("Mode Auto");
      Serial.println("-------------------------------------");
      if (humidity < 5000 && !isOpen) {
        Serial.println("Humidité dans le mode auto : " + String(humidity));
        Serial.println(" FERME, mode Auto");
        openRotate(ROTATE);
      } else if (humidity >= 5000 && isOpen) {
        Serial.println("Humidité dans le mode manuel : " + String(humidity));
        Serial.println(" OUVRE, mode Auto");
        closeRotate(ROTATE);
      } else {
        Serial.println("Je fais rien");
      }
      break;
  }
}

// Fonction calcul vent :

void captWind() {
  if (tActu - tPrec > 0) {
    tPer = tActu - tPrec;
    wind = (2.4 * 1000) / tPer;
  }
  tPrec = tActu;
  if (millis() - tActu > 5000 ) { // bug de la vitesse qui descend pas en dessous de 1.3 km/h
    wind = 0;
  }
}

void inter() {
  unsigned long date = millis();
  if (date - tActu > 5) {
    tPrec = tActu ;
    tActu = date;
  }
}

// --------------------------------------------------

void openRotate(int degree) { // Fonction pour ouvrir le volet
  myMotor->step(ROTATE, FORWARD, MICROSTEP);
  voletPos += degree;
  isOpen = true;
  Serial.println("Mon volet est à la position :" + String(voletPos));
}

void closeRotate(int degree) { // Fonction pour fermer le volet
  myMotor->step(ROTATE, BACKWARD, MICROSTEP);
  voletPos -= degree;
  isOpen = false;
  Serial.println("Mon volet est à la position :" + String(voletPos));
}


void sendHT () {
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Echec de lecture !");
    return;
  }
  byte payload[8];
  payload[0] = highByte(humidity);
  payload[1] = lowByte(humidity);
  payload[2] = highByte(temperature);
  payload[3] = lowByte(temperature);
  if (isOpen) {
    payload[4] = highByte(0);
    payload[5] = lowByte(0);
  } else {
    payload[4] = highByte(1);
    payload[5] = lowByte(1);
  }
  payload[6] = highByte(wind);
  payload[7] = lowByte(wind);
  ttn.sendBytes(payload, sizeof(payload));
  float hi = dht.computeHeatIndex(humidity);

}
