#include "realas5600sensor.h"
#include <iostream>
#include <iomanip>

RealAS5600Sensor::RealAS5600Sensor(int bus, uint8_t deviceAddr)
    : m_bus(bus)
    , m_deviceAddr(deviceAddr)
{
    std::cout << "[RealAS5600Sensor] Initialized on I2C bus " << bus
              << " - Device address 0x" << std::hex << std::uppercase
              << static_cast<int>(deviceAddr) << std::dec << std::endl;
}

std::optional<uint16_t> RealAS5600Sensor::readRawAngle()
{
    try {
        auto data = m_bus.readI2cBlockData(m_deviceAddr, 0x0C, 2);
        if (data.size() == 2) {
            uint16_t angle = (static_cast<uint16_t>(data[0]) << 8) | data[1];
            return angle;
        } else {
            std::cerr << "[RealAS5600Sensor] Raw Angle FAILED - wrong size: "
                      << data.size() << " bytes (expected 2)" << std::endl;
            return std::nullopt;
        }
    } catch (const std::exception& e) {
        std::cerr << "[RealAS5600Sensor] EXCEPTION readRawAngle(): " << e.what() << std::endl;
        return std::nullopt;
    } catch (...) {
        std::cerr << "[RealAS5600Sensor] UNKNOWN EXCEPTION readRawAngle()" << std::endl;
        return std::nullopt;
    }
}

std::optional<uint16_t> RealAS5600Sensor::readMagnitude()
{
    try {
        auto data = m_bus.readI2cBlockData(m_deviceAddr, 0x1B, 2);
        if (data.size() == 2) {
            uint16_t mag = (static_cast<uint16_t>(data[0]) << 8) | data[1];
            return mag;
        } else {
            std::cerr << "[RealAS5600Sensor] Magnitude FAILED - wrong size: "
                      << data.size() << " bytes (expected 2)" << std::endl;
            return std::nullopt;
        }
    } catch (const std::exception& e) {
        std::cerr << "[RealAS5600Sensor] EXCEPTION readMagnitude(): " << e.what() << std::endl;
        return std::nullopt;
    } catch (...) {
        std::cerr << "[RealAS5600Sensor] UNKNOWN EXCEPTION readMagnitude()" << std::endl;
        return std::nullopt;
    }
}

std::optional<uint8_t> RealAS5600Sensor::readStatus()
{
    try {
        auto data = m_bus.readI2cBlockData(m_deviceAddr, 0x0B, 1);
        if (data.size() == 1) {
            uint8_t status = data[0];
            return status;
        } else {
            std::cerr << "[RealAS5600Sensor] STATUS FAILED - wrong size: "
                      << data.size() << " bytes (expected 1)" << std::endl;
            return std::nullopt;
        }
    } catch (const std::exception& e) {
        std::cerr << "[RealAS5600Sensor] EXCEPTION readStatus(): " << e.what() << std::endl;
        return std::nullopt;
    } catch (...) {
        std::cerr << "[RealAS5600Sensor] UNKNOWN EXCEPTION readStatus()" << std::endl;
        return std::nullopt;
    }
}

std::optional<double> RealAS5600Sensor::readAngleDegrees()
{
    if (auto raw = readRawAngle())
        return static_cast<double>(*raw) * 360.0 / 4096.0;
    return std::nullopt;
}