// === i2c/as5600sensor.h ===
#ifndef AS5600SENSOR_H
#define AS5600SENSOR_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <qqmlintegration.h>
#include <QtQml/qqmlregistration.h>

#include "ias5600sensor.h"

class AS5600Sensor : public QObject
{
    Q_OBJECT
    QML_ELEMENT                          // ← auto-registers as QML type in your module
    Q_PROPERTY(double angle READ angle NOTIFY angleChanged FINAL)
    Q_PROPERTY(int rawAngle READ rawAngle NOTIFY rawAngleChanged FINAL)
    Q_PROPERTY(int magnitude READ magnitude NOTIFY magnitudeChanged FINAL)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged FINAL)

public:
    explicit AS5600Sensor(QObject *parent = nullptr);
    ~AS5600Sensor() override = default;

    double angle() const;
    int rawAngle() const;
    int magnitude() const;
    bool active() const;

public slots:
    void setActive(bool active);

signals:
    void angleChanged();
    void rawAngleChanged();
    void magnitudeChanged();
    void activeChanged();

private slots:
    void pollSensor();

private:
    std::unique_ptr<IAS5600Sensor> m_backend;
    QTimer m_timer;

    double m_angle = 0.0;
    int m_rawAngle = 0;
    int m_magnitude = 0;
    bool m_active = true;
};

#endif // AS5600SENSOR_H
