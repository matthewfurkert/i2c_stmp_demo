#ifndef IAS5600_H
#define IAS5600_H

#include <optional>
#include <cstdint>

class IAS5600Sensor {
public:
    virtual ~IAS5600Sensor() = default;

    virtual std::optional<uint16_t> readRawAngle() = 0;
    virtual std::optional<uint16_t> readMagnitude() = 0; // magnetic field strength value
    virtual std::optional<uint8_t>  readStatus()   = 0;
    virtual std::optional<double>   readAngleDegrees() = 0;
};

#endif // IAS5600_H
