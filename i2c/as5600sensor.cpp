// === i2c/as5600sensor.cpp ===
#include "as5600sensor.h"
#include <cmath>
#include <QDebug>

// Conditional includes for real vs mock backend
#ifdef USE_REAL_I2C
#include "realas5600sensor.h"
#else
#include "mockas5600sensor.h"
#endif

AS5600Sensor::AS5600Sensor(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(40); // 25 Hz → smooth dial
    connect(&m_timer, &QTimer::timeout, this, &AS5600Sensor::pollSensor);

#ifdef USE_REAL_I2C
    qDebug() << "=== AS5600Sensor MODE: REAL I2C (Hardware) ===";
    createBackend();
#else
    qDebug() << "=== AS5600Sensor MODE: MOCK (Simulation) ===";
    m_backend = std::make_unique<MockAS5600Sensor>();
#endif

    m_timer.start();
    pollSensor(); // immediate first read
}

void AS5600Sensor::createBackend()
{
#ifdef USE_REAL_I2C
    m_backend = std::make_unique<RealAS5600Sensor>(0, static_cast<uint8_t>(m_deviceAddress));
#endif
}

void AS5600Sensor::pollSensor()
{
    if (!m_active)
        return;

    const int newRaw = static_cast<int>(m_backend->readRawAngle());
    const int newMag = static_cast<int>(m_backend->readMagnitude());
    const int newStatus = static_cast<int>(m_backend->readStatus());
    const double newAngle = m_backend->readAngleDegrees();

    if (newRaw != m_rawAngle) {
        m_rawAngle = newRaw;
        emit rawAngleChanged();
    }
    if (newMag != m_magnitude) {
        m_magnitude = newMag;
        emit magnitudeChanged();
    }
    if (newStatus != m_status) {
        m_status = newStatus;
        emit statusChanged();
    }
    if (std::fabs(newAngle - m_angle) > 0.05) {
        m_angle = newAngle;
        emit angleChanged();
    }
}

double AS5600Sensor::angle() const { return m_angle; }
int AS5600Sensor::rawAngle() const { return m_rawAngle; }
int AS5600Sensor::magnitude() const { return m_magnitude; }
int AS5600Sensor::status() const { return m_status; }
int AS5600Sensor::deviceAddress() const { return m_deviceAddress; }

void AS5600Sensor::setDeviceAddress(int address)
{
    if (address == m_deviceAddress)
        return;

    if (address < 0 || address > 127) {
        qWarning() << "Invalid I2C address (must be 0-127):" << Qt::hex << address;
        return;
    }
    m_deviceAddress = static_cast<uint8_t>(address);

#ifdef USE_REAL_I2C
    createBackend();   // recreate with the new address (instant & safe)
    qDebug() << "AS5600Sensor: Device address changed to 0x" << Qt::hex << address;
#endif

    emit deviceAddressChanged();
}

bool AS5600Sensor::active() const { return m_active; }

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
