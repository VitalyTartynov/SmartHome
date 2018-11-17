/*
 *  This sketch for basic temperature and humidity sensor.
 *  Prepared for ThingsBoard server.
 */

#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// WiFi settings
#define WIFI_SSID "HOMENET"
#define WIFI_PASSWORD "2TarElka"

// DHT sensor settings
#define DHT_PIN 4 // LoLin pin D2
#define DHT_TYPE DHT22

// ThingsBoard server settings
char THINGSBOARD_HOST[] = "192.168.1.101";
#define THINGSBOARD_PORT 1883

// ThingsBoard device settings
#define THINGSBOARD_DEVICE_ID "Hall sensor"
#define THINGSBOARD_DEVICE_TOKEN "UvYkk1TcD3xZcWOMPw0p"

WiFiClient wifiClient;

// Initialize DHT sensor.
DHT dht(DHT_PIN, DHT_TYPE);

// MQTT client.
PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(10);
  InitWiFi();
  client.setServer(THINGSBOARD_HOST, THINGSBOARD_PORT);
  lastSend = 0;
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }

  // Update and send only after 5 seconds
  if (millis() - lastSend > 5000) {
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }

  client.loop();
}

void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting sensors telemetry.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float heatIndex = dht.computeHeatIndex(t, h, false);

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Heat index: ");
  Serial.print(heatIndex);
  Serial.print(" \t");

  String temperature = String(t);
  String humidity = String(h);

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
  client.publish("v1/devices/me/telemetry", attributes);
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
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect(THINGSBOARD_DEVICE_ID, THINGSBOARD_DEVICE_TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
