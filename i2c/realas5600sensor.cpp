#include "realas5600sensor.h"
#include <QDebug>

RealAS5600Sensor::RealAS5600Sensor(int bus, uint8_t deviceAddr)
    : m_bus(bus)
    , m_deviceAddr(deviceAddr)
{
    qDebug() << "[RealAS5600Sensor] Initialized on I2C bus" << bus
             << "- Device address 0x" << Qt::hex << deviceAddr;
}

uint16_t RealAS5600Sensor::readRawAngle() {
    try {
        qDebug() << "[RealAS5600Sensor] Reading Raw Angle (register 0x0C)...";
        auto data = m_bus.readI2cBlockData(m_deviceAddr, 0x0C, 2);

        if (data.size() == 2) {
            uint16_t angle = (static_cast<uint16_t>(data[0]) << 8) | data[1];
            qDebug() << "[RealAS5600Sensor] → Raw Angle SUCCESS:" << angle;
            return angle;
        } else {
            qWarning() << "[RealAS5600Sensor] Raw Angle FAILED - wrong size:" << data.size() << "bytes (expected 2)";
            return 0;
        }
    } catch (const std::exception& e) {
        qWarning() << "[RealAS5600Sensor] EXCEPTION in readRawAngle():" << e.what();
    } catch (...) {
        qWarning() << "[RealAS5600Sensor] UNKNOWN EXCEPTION in readRawAngle()";
    }
    return 0;
}

uint16_t RealAS5600Sensor::readMagnitude() {
    try {
        qDebug() << "[RealAS5600Sensor] Reading Magnitude (register 0x1B)...";
        auto data = m_bus.readI2cBlockData(m_deviceAddr, 0x1B, 2);

        if (data.size() == 2) {
            uint16_t mag = (static_cast<uint16_t>(data[0]) << 8) | data[1];
            qDebug() << "[RealAS5600Sensor] → Magnitude SUCCESS:" << mag;
            return mag;
        } else {
            qWarning() << "[RealAS5600Sensor] Magnitude FAILED - wrong size:" << data.size() << "bytes (expected 2)";
            return 0;
        }
    } catch (const std::exception& e) {
        qWarning() << "[RealAS5600Sensor] EXCEPTION in readMagnitude():" << e.what();
    } catch (...) {
        qWarning() << "[RealAS5600Sensor] UNKNOWN EXCEPTION in readMagnitude()";
    }
    return 0;
}

uint8_t RealAS5600Sensor::readStatus() {
    try {
        qDebug() << "[RealAS5600Sensor] Reading STATUS (register 0x0B)...";
        auto data = m_bus.readI2cBlockData(m_deviceAddr, 0x0B, 1);
        if (data.size() == 1) {
            uint8_t status = data[0];
            qDebug() << "[RealAS5600Sensor] → STATUS SUCCESS:" << status;
            return status;
        } else {
            qWarning() << "[RealAS5600Sensor] STATUS FAILED - wrong size:" << data.size() << "bytes (expected 1)";
            return 0;
        }
    } catch (const std::exception& e) {
        qWarning() << "[RealAS5600Sensor] EXCEPTION in readStatus():" << e.what();
    } catch (...) {
        qWarning() << "[RealAS5600Sensor] UNKNOWN EXCEPTION in readStatus()";
    }
    return 0;
}

double RealAS5600Sensor::readAngleDegrees() {
    return static_cast<double>(readRawAngle()) * 360.0 / 4096.0;
}
