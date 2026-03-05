#ifndef MOCKAS5600SENSOR_H
#define MOCKAS5600SENSOR_H

#include "ias5600sensor.h"
#include <chrono>

class MockAS5600Sensor : public IAS5600Sensor {
public:
    MockAS5600Sensor();

    uint16_t readRawAngle() override;
    uint16_t readMagnitude() override;
    uint8_t readStatus() override;
    double readAngleDegrees() override;

private:
    void updateSimulation();

    uint16_t m_rawAngle = 0;
    uint16_t m_magnitude = 2048;    // typical mid-range value
    uint8_t m_status = 32;         // Good manget strength
    std::chrono::steady_clock::time_point m_lastUpdate;
    const double m_speedDegPerSec = 30.0; // smooth rotation for demo (1 rev ~12 s)
};

#endif // MOCKAS5600SENSOR_H
