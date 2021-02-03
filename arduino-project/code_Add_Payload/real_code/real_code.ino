#include <DHT.h>
#include <DHT_U.h>
#include <TheThingsNetwork.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// ** NE PAS UTILISER LE PIN 2 AVEC LEONARDO POUR CONNEXION TTN **
// ** OUVERTURE FERMETURE : ROTATE DE 4000 POUR DU SINGLE STEP  **

#define CAPTVENT 7 // PIN Branchement du capteur de vent 
#define DHTPIN 5    // PIN Branchement du capteur hum / temp
#define DHTTYPE DHT22       // DHT 22  (AM2302)

#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868


const char *appEui = "70B3D57ED003AE78";
const char *appKey = "20E15BFC27CD9836F96A16D16C3FE6C0";
const int MAX_FERME = 0; 
const int MAX_ROTATION = 90; 
const int ROTATE = 4000; // Tourne le moteur pour ouvrir
const int ROTATECLOSE = 5000;
const int SENSOR = 0; // CAPTEUR HUMIDITY DANS TERRE
int VALUE = 0; // 

//  /!\ De base un boolean est à FALSE /!\ 

boolean autoMode = true;

// Calcul du vent

int vitesse;
unsigned long tPer;
volatile unsigned long tActu, tPrec;

// ----------------------------------

// Savoir si le volet est ouvert ou fermé

boolean isOpen = false;
int voletPos = 0; // Recupération de la position du volet

// ----------------------------------

// Cree un objet motor avec les I2C address defautl
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// TIPS : 200 STEPS = UN TOUR COMPLET DU MOTEUR
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


DHT dht(DHTPIN, DHTTYPE);
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

// Lecture du taux d'humidité
// unit32_t is a numeric type that guarantees 32 bits
uint32_t humidity;

// Lecture de la température en Celcius
uint32_t temperature;

// Lecture du vent
uint32_t wind;

// Lecture de l'humidité dans la terre
uint32_t humiditySoil;

void setup()
{
  pinMode(CAPTVENT, INPUT); // Capteur Vent
  attachInterrupt(digitalPinToInterrupt(CAPTVENT), inter, RISING); // Interrupt Capt.Vent
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  while (!debugSerial && millis() < 10000);
  debugSerial.println("-- STATUS");
  ttn.showStatus();
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
  ttn.onMessage(message);
  dht.begin();
  AFMS.begin();  // Cree avec par default la frequence 1.6KHz
  myMotor->setSpeed(125);  // Donne la speedBase du moteur 10 rpm
}

void loop()
{
  Serial.println("Depart de boucle");
  humidity = dht.readHumidity(false) * 100;
  temperature = dht.readTemperature(false) * 100;
  
  sendHT(); // Données envoyé par les capteurs HUMI / TEMP
  delay(6000);
 
  humiSoil();
  captWind();
  if (wind > 10 && isOpen && !autoMode){
    Serial.println("FERME à cause du vent et mode manuel");
    closeRotate(ROTATECLOSE);
    }
  if (autoMode == true) {
    autoModeAct();
  }
}

// -----------------ENVOIE MESSAGE----------------------

void message (const uint8_t *payload, size_t size, port_t port) {
  switch (payload[0]) {
    case 0: autoMode = false;
      if (!isOpen && wind < 10) {
        Serial.println(" OUVRE, mode MANUEL");
        openRotate(ROTATE);
      } else {
        Serial.println("Je ne peux pas OUVRIR."); break;
      }; break;
    case 1: autoMode = false;
      if (isOpen) {
        Serial.println(" FERME, mode MANUEL");
        closeRotate(ROTATE);
      } else {
        Serial.println("Je ne peux pas FERMER."); break;
      } break;
    case 2:
      autoMode = true;
      break ;
  }
}

// -----------------Fonction calcul HUMIDSOIL----------------------

int humiSoil () {
  Serial.print("MA VALEUR EST DE :  ");
  VALUE = analogRead(SENSOR);
  VALUE = VALUE / 10;
  if (VALUE < 50) { 
    Serial.println("Je suis sec eh");
  } else  {
    Serial.println("JE SUIS TREMPE");
  }
}

// ------------------Fonction calcul VENT-------------------------

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

// -----------------OUVERTURE FERMETURE VOLET -----------------

void openRotate(int degree) {
  myMotor->step(ROTATE, BACKWARD, SINGLE);
  Serial.println("J'ai finis d'ouvrir");
  voletPos -= degree;
  isOpen = true;
  Serial.println("Mon volet est à la position :" + String(voletPos));
}

void closeRotate(int degree) { 
  myMotor->step(ROTATE, FORWARD, SINGLE);
  Serial.println("J'ai finis de fermer");
  voletPos += degree;
  isOpen = false;
  Serial.println("Mon volet est à la position :" + String(voletPos));
}

// -----------------ENVOIE PAYLOAD---------------------------------

void sendHT () {
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Echec de lecture !");
    return;
  }
  byte payload[12];
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
  payload[8] = highByte(autoMode);
  payload[9] = lowByte(autoMode);
  
  payload[10] = highByte(VALUE);
  payload[11] = lowByte(VALUE);


  ttn.sendBytes(payload, sizeof(payload));
  float hi = dht.computeHeatIndex(humidity);
}

// ------------------AUTOMODE--------------------------------

void autoModeAct() {
  Serial.println("-------------------------------------");
  Serial.println("             Mode Auto               ");
  Serial.println("-------------------------------------");
  Serial.println("Humidité dans le mode auto : " + String(humidity));
  if (humidity < 6500 && isOpen) {
    Serial.println(" FERME, mode AUTO");
    closeRotate(ROTATE);
  } else if (humidity >= 6500 && !isOpen && wind < 10) {
    Serial.println(" OUVRE, mode AUTO");
    openRotate(ROTATE);
  } else {
    Serial.println("Je fais rien");
  }
}

// --------------------------------------------------
