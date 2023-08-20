#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <MQTT.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

// Configuration for connect to MQTT server
#define BROKER_IP "MQTT-SERVER"
#define BROKER_PORT 1883
#define DEV_NAME "Battery_Monitoring"
#define MQTT_USER "esp32BMS"
#define MQTT_PW "esp32BMS"
#define VOLT_SENSOR 34

// Set your WiFi here
const char ssid[] = "Rentha";
const char pass[] = "apahayoo";

WiFiClient net;
MQTTClient client;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(4, DHT11);

unsigned long publishTime = 0;
unsigned long samplingTime = 0;
unsigned long displayTime = 0;
float adcRead = 0;
float adcReal = 0;
float voltRead = 0;
float voltReal = 0;
float temp = 0;
int humidity = 0;
char voltValue[8];
char tempValue[8];
char humiValue[8];
char lcdUp[16];
char lcdDown[16];

// Connect to MQTT server
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect(DEV_NAME, MQTT_USER, MQTT_PW)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
}

void setup() {
  pinMode(VOLT_SENSOR, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  dht.begin();
  WiFi.begin(ssid, pass);
  client.begin(BROKER_IP, BROKER_PORT, net);
  // client.onMessage(messageReceived);
  connect();
  client.publish("status", "Connected!");
}

void loop() {
  temp = dht.readTemperature();
  humidity = dht.readHumidity();

  lcd.setCursor(0, 0);
  sprintf(lcdUp, "Voltage: %.1fv", voltReal);
  lcd.print(lcdUp);
  lcd.setCursor(0, 1);
  sprintf(lcdDown, "Hum: %d  Tem: %.0f", humidity, temp);
  lcd.print(lcdDown);

  if (millis() - publishTime >= 5000) {
    client.publish("Voltage", voltValue);
    client.publish("Temp", tempValue);
    client.publish("Humidity", humiValue);
    // Serial.println("Publish");

    publishTime = millis();
  }
  if (millis() - samplingTime >= 500) {
    // Read ADC Value
    adcRead = analogRead(VOLT_SENSOR);

    // ADC reading tolerance on esp32. Each device maybe has a different tolerance.
    adcReal = adcRead + (adcRead * (7.4 / 100));

    // Calculate voltage that received from ADC Value (ADC_Value * Max_Voltage_on_ADC) / ADC_Resolution
    voltRead = (adcReal * 3.3) / 4095;

    // Calculate voltage real from voltage divider (ADC_Voltage_Value / Voltage_Divider_Value)
    voltReal = voltRead / (1 / 21.1);
    dtostrf(voltReal, 1, 2, voltValue);
    dtostrf(temp, 1, 2, tempValue);
    dtostrf(humidity, 1, 2, humiValue);

    samplingTime = millis();
  }

  client.loop();
  if (!client.connected()) {
    connect();
  }
}
