#ifndef RuuviMeasurement_hpp
#define RuuviMeasurement_hpp
#include <Arduino.h>
#include <string>
#include <sstream>
#include <vector>

class RuuviMeasurement {
    private:
        int payloadVersion;
        float temperature; // in ˚C
        float humidity; //relative humidity in %
        float pressure; //in Pascal
        float accelerationX;
        float accelerationY;
        float accelerationZ;
        int battery; // in millivolts
        int txPower; // in dBm
        int movementCount;
        long sequenceNumber;
        uint16_t getUShort(std::vector<uint8_t> data, int index);
        int16_t getShort(std::vector<uint8_t> data, int index);
    public:
        RuuviMeasurement(std::string manufacturerData);
        RuuviMeasurement(RuuviMeasurement &copy);
        RuuviMeasurement();
        ~RuuviMeasurement();
        int getPayloadVersion();
        float getTemperature();
        float getHumidity();
        float getPressure();
        float getAccelerationX();
        float getAccelerationY();
        float getAccelerationZ();
        int getBattery();
        int getTxPower();
        int getMovementCount();
        long getSequenceNumber();
        std::string toString();
};
#endif