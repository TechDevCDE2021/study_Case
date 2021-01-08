#include <DHT.h>
#include <DHT_U.h>
#include <TheThingsNetwork.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

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
boolean isOpen = true;
int voletPos = 0;


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


void setup()
{
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
}

void message(const uint8_t *payload, size_t size, port_t port) {
  if (payload[0] == 1 || payload[0] == 0 ) {
    Serial.println("-------------------------------------");
    Serial.println("Mode Manuel");
    if ( payload[0] == 1 && isOpen) {
      Serial.println("Je tourne pour ouvrir, mode Manuel");
      openRotate(ROTATE);
    } else {
      Serial.println("Je tourne pour fermer, mode Manuel");
      closeRotate(ROTATE);
    }
  } else {
    Serial.println("-------------------------------------");
    Serial.println("Mode Auto");
    Serial.println("Je rentre dans le else");
    Serial.println("-------------------------------------");
    if (humidity >= 5000 && !isOpen) {
      Serial.println("Je tourne pour ouvrir, mode Auto");
      Serial.println("Humidité dans le mode auto : " + String(humidity));
      openRotate(ROTATE);
    } else if (humidity < 5000 && isOpen) {
      Serial.println("Humidité dans le mode manuel : " + String(humidity));
      Serial.println("Je tourne pour fermer, mode Auto");
      closeRotate(ROTATE);
    } else {
      Serial.println("Je fais rien");
    }
  }
  Serial.println("-------------------------------------");
}

void openRotate(int degree) { // Fonction pour ouvrir le volet
  myMotor->step(ROTATE, FORWARD, SINGLE);
  voletPos += degree;
  isOpen = true;
  Serial.println("Mon volet est à la position :" + String(voletPos));
}

void closeRotate(int degree) { // Fonction pour fermer le volet
  myMotor->step(ROTATE, BACKWARD, SINGLE);
  voletPos -= degree;
  isOpen = false;
  Serial.println("Mon volet est à la position :" + String(voletPos));
}


void sendHT () {
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Echec de lecture !");
    return;
  }
  byte payload[6];
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
  ttn.sendBytes(payload, sizeof(payload));
  // float hi = dht.computeHeatIndex(humidity);

}
