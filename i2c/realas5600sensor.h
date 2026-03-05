#ifndef REALAS5600SENSOR_H
#define REALAS5600SENSOR_H

#include "ias5600sensor.h"
#include "smbus.hpp"

class RealAS5600Sensor : public IAS5600Sensor {
public:
    // Device address (default = 0x40 for AS5600L)
    explicit RealAS5600Sensor(int bus = 0, uint8_t deviceAddr = 0x40);

    uint16_t readRawAngle() override;
    uint16_t readMagnitude() override;
    uint8_t readStatus() override;
    double readAngleDegrees() override;

private:
    SMBus m_bus;
    uint8_t m_deviceAddr;
};

#endif // REALAS5600SENSOR_H
