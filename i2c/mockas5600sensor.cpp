#include "mockas5600sensor.h"
#include <cmath>

MockAS5600Sensor::MockAS5600Sensor()
    : m_lastUpdate(std::chrono::steady_clock::now())
{}

void MockAS5600Sensor::updateSimulation() {
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - m_lastUpdate).count();
    if (dt <= 0.0) return;

    double currentDeg = static_cast<double>(m_rawAngle) * 360.0 / 4096.0;
    currentDeg = std::fmod(currentDeg + m_speedDegPerSec * dt, 360.0);
    if (currentDeg < 0.0) currentDeg += 360.0;

    m_rawAngle = static_cast<uint16_t>(currentDeg * 4096.0 / 360.0) % 4096;
    m_lastUpdate = now;
}

uint16_t MockAS5600Sensor::readRawAngle() {
    updateSimulation();
    return m_rawAngle;
}

uint16_t MockAS5600Sensor::readMagnitude() {
    updateSimulation();
    return m_magnitude;
}

double MockAS5600Sensor::readAngleDegrees() {
    updateSimulation();
    return static_cast<double>(m_rawAngle) * 360.0 / 4096.0;
}
