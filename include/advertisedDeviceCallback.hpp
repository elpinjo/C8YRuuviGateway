#ifndef ADVERTISED_DEVICE_CALLBACK_HPP
#define ADVERTISED_DEVICE_CALLBACK_HPP

#include <Arduino.h>
#include <BLEAdvertisedDevice.h>
#include <BLEUtils.h>
#include <PubSubClient.h>
#include <map>
#include <string>
#include "ruuvi.hpp"

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    public:
        MyAdvertisedDeviceCallbacks();
        void onResult(BLEAdvertisedDevice advertisedDevice);
        void setMqttClient(PubSubClient* aClient);
    private:
        PubSubClient* mqttClient;
        std::map<std::string, long> historicSequences;
        std::map<std::string, long> historicMovement;
};

#endif