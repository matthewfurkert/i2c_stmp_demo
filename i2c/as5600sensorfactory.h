#ifndef AS5600SENSORFACTORY_H
#define AS5600SENSORFACTORY_H

#include "ias5600sensor.h"
#include <memory>
#include <QDebug>

#ifdef USE_REAL_I2C
#include "realas5600sensor.h"
#else
#include "mockas5600sensor.h"
#endif

class AS5600SensorFactory {
public:
    static std::unique_ptr<IAS5600Sensor> create() {
#ifdef USE_REAL_I2C
        qDebug() << "=== AS5600Sensor MODE: REAL I2C (Hardware) ===";
        return std::make_unique<RealAS5600Sensor>(3);   // change 3 if your bus is different
#else
        qDebug() << "=== AS5600Sensor MODE: MOCK (Simulation) ===";
        return std::make_unique<MockAS5600Sensor>();
#endif
    }
};

#endif // AS5600SENSORFACTORY_H
