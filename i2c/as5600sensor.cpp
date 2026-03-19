#include "as5600sensor.h"
#include <cmath>
#include <QDebug>

#ifdef USE_REAL_I2C
#include "realas5600sensor.h"
#else
#include "mockas5600sensor.h"
#endif

AS5600Sensor::AS5600Sensor(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(40); // 25 Hz
    connect(&m_timer, &QTimer::timeout, this, &AS5600Sensor::pollSensor);
    m_timer.start();
}

void AS5600Sensor::createBackend()
{
#ifdef USE_REAL_I2C
    m_backend = std::make_unique<RealAS5600Sensor>(m_busAddress, static_cast<uint8_t>(m_deviceAddress));
#else
    m_backend = std::make_unique<MockAS5600Sensor>();
#endif
}

void AS5600Sensor::pollSensor()
{
    if (!m_backend) {
        if (m_busAddress >= 0 && m_deviceAddress >= 0 && m_deviceAddress <= 127) {
            createBackend();
        } else {
            // Changed to qWarning() as requested (console only, no QML banner spam)
            qWarning() << "AS5600Sensor: Backend not created - both busAddress (>= 0) and deviceAddress (0-127) must be set first.";
            qWarning() << "   Current values: busAddress =" << m_busAddress
                       << ", deviceAddress = 0x" << Qt::hex << m_deviceAddress;
            return;
        }
    }

    auto newRaw    = m_backend->readRawAngle();
    auto newMag    = m_backend->readMagnitude();
    auto newStatus = m_backend->readStatus();
    auto newAngle  = m_backend->readAngleDegrees();

    bool commFailed = !newRaw.has_value() || !newMag.has_value() || !newStatus.has_value();
    bool magnetOK   = newStatus.has_value() && (*newStatus & 0x20);
    bool newValid   = !commFailed && magnetOK;

    if (newValid != m_valid) {
        m_valid = newValid;
        emit validChanged();
    }

    QString newError = newValid ? QString()
                                : (commFailed ? "Sensor Not Detected" : "Magnet Not Detected");

    if (newError != m_errorMessage) {
        m_errorMessage = newError;
        emit errorMessageChanged();
    }

    if (!newValid) {
        return;
    }

    if (newRaw && *newRaw != m_rawAngle) {
        m_rawAngle = *newRaw;
        emit rawAngleChanged();
    }
    if (newMag && *newMag != m_magnitude) {
        m_magnitude = *newMag;
        emit magnitudeChanged();
    }
    if (newStatus && *newStatus != m_status) {
        m_status = *newStatus;
        emit statusChanged();
    }
    if (newAngle && std::fabs(*newAngle - m_angle) > 0.05) {
        m_angle = *newAngle;
        emit angleChanged();
    }
}

void AS5600Sensor::setBusAddress(int busAddress)
{
    if (busAddress == m_busAddress)
        return;

    if (busAddress < 0) {
        qWarning() << "Invalid I2C bus number (must be >= 0):" << busAddress;
        return;
    }

    m_busAddress = busAddress;

    if ( m_deviceAddress >= 0 && m_deviceAddress <= 127) {
        createBackend();
    }

    emit busAddressChanged();
}

void AS5600Sensor::setDeviceAddress(int deviceAddress)
{
    if (deviceAddress == m_deviceAddress)
        return;

    if (deviceAddress < 0 || deviceAddress > 127) {
        qWarning() << "Invalid I2C device address (must be 0-127):"
                   << Qt::hex << deviceAddress;
        return;
    }

    m_deviceAddress = deviceAddress;

    if (m_busAddress >= 0)
        createBackend();

    emit deviceAddressChanged();
}

int AS5600Sensor::magnitude() const { return m_magnitude; }
int AS5600Sensor::status() const { return m_status; }
int AS5600Sensor::rawAngle() const { return m_rawAngle; }
double AS5600Sensor::angle() const { return m_angle; }

int AS5600Sensor::busAddress() const { return m_busAddress; }
int AS5600Sensor::deviceAddress() const { return m_deviceAddress; }

bool AS5600Sensor::valid() const { return m_valid; }
QString AS5600Sensor::errorMessage() const { return m_errorMessage; }