#ifndef MOCKAS5600_H
#define MOCKAS5600_H

#include "ias5600.h"

class MockAS5600Sensor : public IAS5600Sensor {
public:
    std::optional<uint16_t> readRawAngle() override {
        updateSimulation();
        return m_rawAngle;
    }
    std::optional<uint16_t> readMagnitude() override { return m_magnitude; }
    std::optional<uint8_t>  readStatus() override { return m_status; }
    std::optional<double>   readAngleDegrees() override {
        return static_cast<double>(m_rawAngle) * 360.0 / 4096.0;
    }

private:
    uint16_t m_rawAngle = 0;
    uint16_t m_magnitude = 2048;
    uint8_t  m_status = 32;
    const uint16_t m_angleStep = 14;   // ≈ 1.23° per poll at 25 Hz

    void updateSimulation() {
        m_rawAngle = (m_rawAngle + m_angleStep) % 4096;
    }
};

#endif // MOCKAS5600_H
