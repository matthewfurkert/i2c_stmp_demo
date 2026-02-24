#ifndef REALAS5600SENSOR_H
#define REALAS5600SENSOR_H

#include "ias5600sensor.h"
#include "smbus.hpp"

class RealAS5600Sensor : public IAS5600Sensor {
public:
    explicit RealAS5600Sensor(int bus = 3);

    uint16_t readRawAngle() override;
    uint16_t readMagnitude() override;
    double readAngleDegrees() override;

private:
    SMBus m_bus;
    static constexpr uint8_t DEVICE_ADDR = 0x40; // AS5600 = 0x36, AS5600L default = 0x40 (change if needed)
};

#endif // REALAS5600SENSOR_H
