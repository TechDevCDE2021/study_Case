#include <DHT.h>
#include <DHT_U.h>

#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const char *appEui = "70B3D57ED003AE78";
const char *appKey = "7BB63039B8E74C82C4C0AAFC2C0B8E21";

#define loraSerial Serial1
#define debugSerial Serial

#define LED_1 4
#define LED_2 5

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

DHT dht(2, DHT22);
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

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
}

void loop()
{
  debugSerial.println("-- LOOP");

  // unit32_t is a numeric type that guarantees 32 bits
  uint32_t humidity = dht.readHumidity(false) * 100;
  uint32_t temperature = dht.readTemperature(false) * 100;

  debugSerial.println("Humidity: " + String(humidity));
  debugSerial.println("temperature: " + String(temperature));


  byte payload[4];
  payload[0] = highByte(humidity);
  payload[1] = lowByte(humidity);
  payload[2] = highByte(temperature);
  payload[3] = lowByte(temperature);

  ttn.sendBytes(payload, sizeof(payload));

  delay(10000);
}

void message(const uint8_t *payload, size_t size, port_t port) {

  if (payload[0] == 1) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
  } else {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
  }
}
