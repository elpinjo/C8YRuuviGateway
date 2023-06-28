#include "ruuvi.hpp"

RuuviMeasurement::RuuviMeasurement() {
    this->sequenceNumber = -1;
}

RuuviMeasurement::RuuviMeasurement(RuuviMeasurement &copy) {
    this->payloadVersion = copy.getPayloadVersion();
    this->temperature = copy.getTemperature();
    this->humidity = copy.getHumidity();
    this->pressure = copy.getPressure();
    this->accelerationX = copy.getAccelerationX();
    this->accelerationY = copy.getAccelerationY();
    this->accelerationZ = copy.getAccelerationZ();
    this->battery = copy.getBattery();
    this->txPower = copy.getTxPower();
    this->movementCount = copy.getMovementCount();
    this->sequenceNumber = copy.getSequenceNumber();
}

RuuviMeasurement::RuuviMeasurement(std::string manufacturerData) {

    std::vector<uint8_t> data(manufacturerData.begin(),manufacturerData.end());

    this->payloadVersion = data[2];

    if (this->payloadVersion == 5) {
        this->temperature = getShort(data, 3) * 0.005;
        this->humidity = getShort(data, 5) * 0.0025;
        this->pressure = (getUShort(data, 7) + 50000)/100.00;
       
        this->accelerationX = getShort(data, 9) / 1000.00;
        this->accelerationY = getShort(data, 11) / 1000.00;
        this->accelerationZ = getShort(data, 13) / 1000.00;

        this->battery = ((int16_t)data[15]<<3 | (int16_t)data[16+1]>>5)+1600;
        this->txPower = ((uint8_t)data[16] & 0x001F)*2-40;
        this->movementCount = data[17];
        this->sequenceNumber = getUShort(data, 18);
    } else {
        Serial.println("Unknown Ruuvi Payload version");
    }
}

RuuviMeasurement::~RuuviMeasurement() {
};


uint16_t RuuviMeasurement::getUShort(std::vector<uint8_t> data, int index){

    return (uint16_t)data[index]<<8 | (uint16_t)data[index+1];
};

int16_t RuuviMeasurement::getShort(std::vector<uint8_t> data, int index){

    return (int16_t)data[index]<<8 | (int16_t)data[index+1];

};

int RuuviMeasurement::getPayloadVersion(){
    return this->payloadVersion;
};

float RuuviMeasurement::getTemperature(){
    return this->temperature;
};

float RuuviMeasurement::getHumidity(){
    return this->humidity;
};

float RuuviMeasurement::getPressure(){
    return this->pressure;
};

float RuuviMeasurement::getAccelerationX(){
    return this->accelerationX;
};

float RuuviMeasurement::getAccelerationY(){
    return this->accelerationY;
};

float RuuviMeasurement::getAccelerationZ(){
    return this->accelerationZ;
};

int RuuviMeasurement::getBattery(){
    return this->battery;
};

int RuuviMeasurement::getTxPower(){
    return this->txPower;
};

int RuuviMeasurement::getMovementCount(){
    return this->movementCount;
};

long RuuviMeasurement::getSequenceNumber(){
    return this->sequenceNumber;
};

std::string RuuviMeasurement::toString() {
    std::stringstream stream;
    stream << "Ruuvi: {\n";
    stream << "\t'temperature'=" << temperature << ",\n";
    stream << "\t'pressure'="    << this->pressure << ",\n";
    stream << "\t'humidity'="    << this->humidity << ",\n";
    stream << "\t'accelX'="      << this->accelerationX << ",\n";
    stream << "\t'accelY'="      << this->accelerationY << ",\n";
    stream << "\t'accelZ'="      << this->accelerationZ << ",\n";
    stream << "\t'battery'="     << this->battery << ",\n";
    stream << "\t'txPower'="     << this->txPower << ",\n";
    stream << "\t'move'="        << this->movementCount << ",\n";
    stream << "\t'sequence'="    << this->sequenceNumber << ",\n}";

    return stream.str();
};