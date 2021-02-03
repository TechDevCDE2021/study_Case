#include <DHT.h>
#include <DHT_U.h>
#include <TheThingsNetwork.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define LECTUREVOLET A2
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
const int ROTATE = 5000; 
const int ROTATECLOSE = 5000; // 5000
const int MINIROTATE = 100;
const int SENSOR = 0; // CAPTEUR HUMIDITY DANS TERRE
const int BATTERIE = 1;
int VALUEBATTERIE = 0; // Pour la batterie
int VALUE = 0; // Pour le capteur hum terre 
int humBound = 6500; // Variable pour gerer le taux d'humidité voulu dans la serre
int contact;
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

// Lecture de la batterie 
uint32_t batterie;

void setup()
{
  pinMode(LECTUREVOLET, INPUT_PULLUP);
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
  //ToDo voir pq le pin 3 nous envoie 1
  Serial.println("Depart de boucle");
  lecBatterie();
  humidity = dht.readHumidity(false) * 100;
  temperature = dht.readTemperature(false) * 100;
  
  sendHT(); // Données envoyé par les capteurs HUMI / TEMP
  delay(6000);
  humiSoil();
  captWind();
  if (wind > 10 && isOpen){
    Serial.println("FERME à cause du vent et mode manuel");
    closeRotate(ROTATECLOSE);
    }
  if (autoMode == true) {
    autoModeAct();
  }
}
//-----------------LECTURE BATTERIE ----------------

int lecBatterie(){
  Serial.print("La batterie fait : ");
  VALUEBATTERIE = analogRead(BATTERIE);
  Serial.println(VALUEBATTERIE);
}

// -----------------ENVOIE MESSAGE----------------------

// -----------------RECEPTION MESSAGE----------------------

void message (const uint8_t *payload, size_t size, port_t port) {
  switch (payload[0]) {
    case 1: autoMode = false;
      if (!isOpen && wind < 10) {
        Serial.println(" OUVRE, mode MANUEL");
        openRotate(ROTATE);
      } else {
        Serial.println("Je ne peux pas OUVRIR."); break;
      }; break;
    case 0: autoMode = false;
      if (isOpen) {
        Serial.println(" FERME, mode MANUEL");
        closeRotate(ROTATECLOSE);
      } else {
        Serial.println("Je ne peux pas FERMER."); break;
      } break;
    case 2:
      autoMode = true;
      humBound = payload[1]*100;
      Serial.print("valeur payload[1] : ");
      Serial.println(payload[1]);
      break ;
  }
}

// -----------------Fonction calcul HUMIDSOIL----------------------

int humiSoil () {
  VALUE = analogRead(SENSOR);
  VALUE = VALUE / 10;
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

// -----------------OUVERTURE et FERMETURE VOLET -----------------

void openRotate(int degree) {
  Serial.println("OUVERTURE DU VOLET : ");
  myMotor->step(ROTATE, BACKWARD, SINGLE);
  Serial.print("OUVERTURE TERMINE");
  voletPos -= degree;
  isOpen = true;
//  Serial.println("Mon volet est à la position :" + String(voletPos));
}

bool isTouch(unsigned test){
  unsigned r = analogRead(test);
  Serial.print("R : "); Serial.println(r);
  if(r > 500 && r < 800) return true;
  else return false;
}

void closeRotate(int degree) {
  Serial.println("FERMETURE DU VOLET : ");
  contact = 0;
  while (!isTouch(LECTUREVOLET)) {
    Serial.println(isTouch(LECTUREVOLET) ? "Touche" : "Touche pas");
    myMotor->step(MINIROTATE, FORWARD, SINGLE);
    Serial.println("FERMETURE EN COURS ");
  } 
  Serial.println("FERMETURE TERMINE");
  voletPos += degree;
  isOpen = false;
//  Serial.println("Mon volet est à la position :" + String(voletPos));
}

// -----------------ENVOIE PAYLOAD---------------------------------

void sendHT () {
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Echec de lecture !");
    return;
  }
  byte payload[14];
  payload[0] = highByte(humidity);
  payload[1] = lowByte(humidity);
  payload[2] = highByte(temperature);
  payload[3] = lowByte(temperature);
  if (isOpen) {
    payload[4] = highByte(isOpen);
    payload[5] = lowByte(isOpen);
  } else {
    payload[4] = highByte(isOpen);
    payload[5] = lowByte(isOpen);
  }
  payload[6] = highByte(wind);
  payload[7] = lowByte(wind);
  payload[8] = highByte(autoMode);
  payload[9] = lowByte(autoMode);
  
  payload[10] = highByte(VALUE);
  payload[11] = lowByte(VALUE);
  payload[12] = highByte(VALUEBATTERIE);
  payload[13] = lowByte(VALUEBATTERIE);

  ttn.sendBytes(payload, sizeof(payload));
  float hi = dht.computeHeatIndex(humidity);
}

// ----------------------------AUTOMODE--------------------------------

void autoModeAct() {
  Serial.println(F("-------------------------------------"));
  Serial.println("             Mode Auto               ");
  Serial.println(F("-------------------------------------"));
  Serial.println("Humidité dans le mode auto : " + String(humidity));
  if (humidity < humBound && isOpen) {
    Serial.println(F(" FERME, mode AUTO"));
    closeRotate(ROTATE);
  } else if (humidity >= humBound && !isOpen && wind < 10) {
    Serial.println(F(" OUVRE, mode AUTO"));
    openRotate(ROTATE);
  } else {
    Serial.println("Je fais rien");
  }
}

// --------------------------------------------------
