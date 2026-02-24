// === i2c/as5600sensor.cpp ===
#include "as5600sensor.h"
#include "as5600sensorfactory.h"
#include <cmath>

AS5600Sensor::AS5600Sensor(QObject *parent)
    : QObject(parent)
    , m_backend(AS5600SensorFactory::create())
{
    m_timer.setInterval(40);                    // 25 Hz → buttery-smooth dial
    connect(&m_timer, &QTimer::timeout, this, &AS5600Sensor::pollSensor);

    m_timer.start();
    pollSensor();                               // immediate first read
}

void AS5600Sensor::pollSensor()
{
    if (!m_active)
        return;

    const int newRaw = static_cast<int>(m_backend->readRawAngle());
    const int newMag = static_cast<int>(m_backend->readMagnitude());
    const double newAngle = m_backend->readAngleDegrees();

    if (newRaw != m_rawAngle) {
        m_rawAngle = newRaw;
        emit rawAngleChanged();
    }
    if (newMag != m_magnitude) {
        m_magnitude = newMag;
        emit magnitudeChanged();
    }
    if (std::fabs(newAngle - m_angle) > 0.05) {
        m_angle = newAngle;
        emit angleChanged();
    }
}

double AS5600Sensor::angle() const          { return m_angle; }
int    AS5600Sensor::rawAngle() const       { return m_rawAngle; }
int    AS5600Sensor::magnitude() const      { return m_magnitude; }
bool   AS5600Sensor::active() const         { return m_active; }

void AS5600Sensor::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    if (m_active)
        m_timer.start();
    else
        m_timer.stop();

    emit activeChanged();
}
