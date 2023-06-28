#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "ruuvi.hpp"
#include "advertisedDeviceCallback.hpp"
#include "credentials.h"

static boolean connected = false;

int scanTime = 5; //In seconds
char clientId[20];

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
BLEScan* pBLEScan;


class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
      Serial.println("onDisconnect");
    }
};

//Serial Number taken from the ESP32, not tested on ESP8266
void getSerialNumber() {

  uint64_t chipid = ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(clientId, 19, "ESP32-%04X%08X", chip, (uint32_t)chipid);

  Serial.printf("Serial Number is: %s\n", clientId);
}

void connectWifi() {
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (tries++ > 10) {
      WiFi.begin(ssid, wifiPassword);
      tries = 0;
    }

  }
  Serial.println("connected to wifi");
}

void reconnectMqtt() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(clientId)) {
      Serial.println("Connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  getSerialNumber();
  connectWifi();
  mqttClient.setServer(host, 1883);
  reconnectMqtt();

  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  MyAdvertisedDeviceCallbacks* myCallbacksObject = new MyAdvertisedDeviceCallbacks();
  myCallbacksObject->setMqttClient(&mqttClient);
  pBLEScan->setAdvertisedDeviceCallbacks(myCallbacksObject);
  pBLEScan->setActiveScan(false); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(1000);
}