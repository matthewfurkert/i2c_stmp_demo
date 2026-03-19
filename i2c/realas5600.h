#ifndef REALAS5600_H
#define REALAS5600_H

#include "ias5600.h"
#include "smbus.hpp"

class RealAS5600Sensor : public IAS5600Sensor {
public:
    // Device address (default = 0x40 for AS5600L)
    explicit RealAS5600Sensor(int bus = 0, uint8_t deviceAddr = 0x40);

    std::optional<uint16_t> readRawAngle() override;
    std::optional<uint16_t> readMagnitude() override;
    std::optional<uint8_t> readStatus() override;
    std::optional<double> readAngleDegrees() override;

private:
    SMBus m_bus;
    uint8_t m_deviceAddr;
};

#endif // REALAS5600_H
