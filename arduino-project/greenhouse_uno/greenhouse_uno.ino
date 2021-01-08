
#include <DHT.h>
#include <DHT_U.h>

#include <TheThingsNetwork.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED003AE78";
const char *appKey = "20E15BFC27CD9836F96A16D16C3FE6C0";

#define DHTPIN 2    // PIN Branchement du capteur hum / temp
#define DHTTYPE DHT22       // DHT 22  (AM2302)

// Cree un objet motor avec les I2C address defautl
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

#define loraSerial Serial1
#define debugSerial Serial
#define LED_1 4
#define LED_2 5
#define freqPlan TTN_FP_EU868

DHT dht(DHTPIN, DHTTYPE);
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

const int MAX_FERME = 0; // TODO  : il faut récupérer la position moteur pour qu'il se ferme jusqu'à un certain point.
const int MAX_ROTATION = 88; // TODO : il faut récupérer la position moteur MAX pour qu'il s'ouvre jusqu'à un certain point.
const int TROIS_QUART = 67;
const int MOITIE_TOUR = 45;
const int QUART_TOUR = 22 ;

int voletPos = 0;

void setup()
{
  //  pinMode(LED_1, OUTPUT);
  //  pinMode(LED_2, OUTPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

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
  debugSerial.println("-- LOOP");

  actifMotor(); // ActifMotor selon les données envoyé par captHT

  delay(10000);
}

void message(const uint8_t *payload, size_t size, port_t port) {

  if (payload[0] == 1) {
    closeRotate(QUART_TOUR);
  } else {
    openRotate(QUART_TOUR);
  }
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
}

void closeRotate(int degree) { // Fonction pour fermer le volet
  myMotor->step(QUART_TOUR, BACKWARD, SINGLE);
  voletPos -= degree;
  Serial.println("Mon volet est à la position :" + String(voletPos));
}

void actifMotor () { // FONCTION PRINCIPAL

  // Lecture du taux d'humidité
  // unit32_t is a numeric type that guarantees 32 bits
  uint32_t humidity = dht.readHumidity(false) * 100;
  // Lecture de la température en Celcius
  uint32_t temperature = dht.readTemperature(false) * 100;
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Echec de lecture !");
    return;
  }

  byte payload[6];
  payload[0] = highByte(humidity);
  payload[1] = lowByte(humidity);
  payload[2] = highByte(temperature);
  payload[3] = lowByte(temperature);
  payload[4] = highByte(temperature);
  payload[5] = lowByte(temperature);

  ttn.sendBytes(payload, sizeof(payload));
  debugSerial.println("Humidity: " + String(humidity));
  debugSerial.println("temperature: " + String(temperature));

  if (voletPos >= MAX_ROTATION && humidity >= 40 ) {
    Serial.println("Le volet ne peut pas être plus ouvert");
  } else if  (voletPos >= MAX_ROTATION && humidity <= 40) {
    if (humidity <= 30 ) {
      closeRotate(MAX_FERME);
    } else {
      closeRotate(QUART_TOUR);
    }
  } else {
    if (humidity >= 50 && humidity <= 69 ) {
      openRotate(QUART_TOUR); // 45

    } else if (humidity >= 70 && humidity <= 84) {
      openRotate(MOITIE_TOUR); // 90

    } else if (humidity > 85 && humidity <= 90) {
      openRotate(TROIS_QUART); // 135

    } else if (humidity >= 91)  {
      openRotate(MAX_ROTATION); // 180

    }
  }

  // Calcul la température ressentie. Il calcul est effectué à partir de la température en Fahrenheit
  // On fait la conversion en Celcius dans la foulée
  float hi = dht.computeHeatIndex(humidity);


  Serial.print("Humidite: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C ");
  Serial.print("Temperature ressentie: ");
  Serial.print(dht.convertFtoC(hi));
  Serial.println(" C");

}
