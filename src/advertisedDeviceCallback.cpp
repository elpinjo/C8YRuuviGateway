#include "advertisedDeviceCallback.hpp"
#include <sstream>

MyAdvertisedDeviceCallbacks::MyAdvertisedDeviceCallbacks() {
  Serial.println("AdvertisedDeviceCallback Created.");
}

void MyAdvertisedDeviceCallbacks::setMqttClient(PubSubClient* aClient){
  mqttClient = aClient;
}

void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) {

    if (advertisedDevice.haveManufacturerData()) {
        std::string bluetoothData = advertisedDevice.getManufacturerData();

        //Manufacturer ID of Ruuvi = 0x9904
        if (bluetoothData[0] == 153 && bluetoothData[1] == 4) {

          std::string ruuviAddress = advertisedDevice.getAddress().toString();
          Serial.printf("a Ruuvi has been found %s\n", ruuviAddress.c_str());

          RuuviMeasurement measurement(bluetoothData);
          if ( historicSequences[ruuviAddress] < measurement.getSequenceNumber()){
            if (historicSequences[ruuviAddress] == 0) {
              Serial.println("Creating a child device");
              mqttClient->publish("c8y/s/us", std::string("101,").append(ruuviAddress).append(",Ruuvi Sensor Tag ").append(ruuviAddress.substr(12)).append(",ruuvi_tag").c_str());
            }
            Serial.println(measurement.toString().c_str());

            historicSequences[ruuviAddress] = measurement.getSequenceNumber();

            std::stringstream stream;
            stream << "901,," << measurement.getTemperature() << ","
                            << measurement.getPressure() << ","
                            << measurement.getHumidity() << ","
                            << measurement.getAccelerationX() << ","
                            << measurement.getAccelerationY() << ","
                            << measurement.getAccelerationZ() << ","
                            << measurement.getBattery() << ","
                            << measurement.getTxPower() << ","
                            << measurement.getMovementCount() << ","
                            << measurement.getSequenceNumber();

            std::string topicString = std::string("c8y/s/uc/ruuvi/").append(ruuviAddress);

            const char* topic = topicString.c_str();
            const char* content = stream.str().c_str();
            
            if (mqttClient == nullptr) {
              Serial.println("Mqtt does not exist.");
            } else {
              Serial.println("Sending measurement to Cumulocity.");
              mqttClient->publish(topic, content);
            }

            // if(historicMovement[ruuviAddress] < measurement.getMovementCount()) {
            //    historicMovement[ruuviAddress] = measurement.getMovementCount();
            //    mqttClient->publish(std::string("c8y/s/us/").append(ruuviAddress).c_str(), "400,c8y_MovementEvent,Movement detected by Ruuvi");
            // }
          }
        }
    }
};