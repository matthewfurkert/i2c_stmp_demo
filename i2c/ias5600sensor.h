#ifndef IAS5600SENSOR_H
#define IAS5600SENSOR_H

#include <cstdint>

class IAS5600Sensor {
public:
    virtual ~IAS5600Sensor() = default;

    virtual uint16_t readRawAngle() = 0;      // 0..4095 (12-bit)
    virtual uint16_t readMagnitude() = 0;     // magnetic field strength
    virtual double readAngleDegrees() = 0;    // convenient 0.0..359.999
};

#endif // IAS5600SENSOR_H
