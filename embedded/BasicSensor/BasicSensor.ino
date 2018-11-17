/*
 *  This sketch for basic temperature and humidity sensor.
 *  Prepared for ThingsBoard server.
 */

#include <DHT.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "Configuration.h"

WiFiClient wifiClient;

// Initialize DHT sensor.
DHT dhtSensor(DHT_PIN, DHT_TYPE);

// MQTT client.
PubSubClient mqttClient(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastTelemetrySend;

void setup()
{
  Serial.begin(115200);
  dhtSensor.begin();
  delay(10);
  InitWiFi();
  mqttClient.setServer(THINGSBOARD_HOST, THINGSBOARD_PORT);
  lastTelemetrySend = 0;
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }

  // Update and send telemetry.
  if (millis() - lastTelemetrySend > THINGSBOARD_DEVICE_TELEMETRY_REFRESH) {
    getAndSendTemperatureAndHumidityData();
    lastTelemetrySend = millis();
  }

  mqttClient.loop();
}

void getAndSendTemperatureAndHumidityData()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dhtSensor.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dhtSensor.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read telemetry from DHT sensor!");
    return;
  }

  float heatIndex = dhtSensor.computeHeatIndex(temperature, humidity, false);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Heat index: ");
  Serial.print(heatIndex);
  Serial.print(" \t");

  Serial.print("  -> ");

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"Temperature\":"; payload += temperature; payload += ",";
  payload += "\"Humidity\":"; payload += humidity; payload += ",";
  payload += "\"HeatIndex\":"; payload += heatIndex;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray(attributes, 100);
  mqttClient.publish("v1/devices/me/telemetry", attributes);
  Serial.println(attributes);
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    status = WiFi.status();
    if (status != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if (mqttClient.connect(THINGSBOARD_DEVICE_ID, THINGSBOARD_DEVICE_TOKEN, NULL)) {
      Serial.println("[DONE]");
    } else {
      Serial.print("[FAILED] [ rc = ");
      Serial.print(mqttClient.state());
      Serial.println(" : retrying in 5 seconds]");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
