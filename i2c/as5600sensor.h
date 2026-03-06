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
    QML_ELEMENT
    Q_PROPERTY(double angle READ angle NOTIFY angleChanged FINAL)
    Q_PROPERTY(int rawAngle READ rawAngle NOTIFY rawAngleChanged FINAL)
    Q_PROPERTY(int magnitude READ magnitude NOTIFY magnitudeChanged FINAL)
    Q_PROPERTY(int status READ status NOTIFY statusChanged FINAL)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged FINAL)
    Q_PROPERTY(int deviceAddress READ deviceAddress WRITE setDeviceAddress NOTIFY deviceAddressChanged)

public:
    explicit AS5600Sensor(QObject *parent = nullptr);
    ~AS5600Sensor() override = default;

    double angle() const;
    int rawAngle() const;
    int magnitude() const;
    int status() const;
    bool active() const;
    int deviceAddress() const;

public slots:
    void setActive(bool active);
    void setDeviceAddress(int address);

signals:
    void angleChanged();
    void rawAngleChanged();
    void magnitudeChanged();
    void statusChanged();
    void activeChanged();
    void deviceAddressChanged();

private slots:
    void pollSensor();

private:
    std::unique_ptr<IAS5600Sensor> m_backend;
    QTimer m_timer;

    double m_angle = 0.0;
    int m_rawAngle = 0;
    int m_magnitude = 0;
    int m_status = 0;
    bool m_active = true;
    uint8_t m_deviceAddress = 0x40;
    void createBackend();
};

#endif // AS5600SENSOR_H
