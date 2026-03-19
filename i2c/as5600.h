// === i2c/as5600sensor.h ===
#ifndef AS5600_H
#define AS5600_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <qqmlintegration.h>
#include <QtQml/qqmlregistration.h>

#include "ias5600.h"

class AS5600Sensor : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int magnitude READ magnitude NOTIFY magnitudeChanged FINAL)
    Q_PROPERTY(int status READ status NOTIFY statusChanged FINAL)
    Q_PROPERTY(int rawAngle READ rawAngle NOTIFY rawAngleChanged FINAL)
    Q_PROPERTY(double angle READ angle NOTIFY angleChanged FINAL)

    Q_PROPERTY(int busAddress READ busAddress WRITE setBusAddress NOTIFY busAddressChanged)
    Q_PROPERTY(int deviceAddress READ deviceAddress WRITE setDeviceAddress NOTIFY deviceAddressChanged)

    Q_PROPERTY(bool valid READ valid NOTIFY validChanged FINAL)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged FINAL)

public:
    explicit AS5600Sensor(QObject *parent = nullptr);
    ~AS5600Sensor() override = default;

    // Getters
    int magnitude() const;
    int status() const;
    int rawAngle() const;
    double angle() const;

    int busAddress() const;
    int deviceAddress() const;

    bool valid() const;
    QString errorMessage() const;

public slots:
    // Setters
    void setBusAddress(int busAddress);
    void setDeviceAddress(int deviceAddress);

signals:
    void magnitudeChanged();
    void statusChanged();
    void rawAngleChanged();
    void angleChanged();

    void busAddressChanged();
    void deviceAddressChanged();

    void validChanged();
    void errorMessageChanged();

private slots:
    void pollSensor();

private:
    int m_magnitude = 0;
    int m_status = 0;
    int m_rawAngle = 0;
    double m_angle = 0.0;

    int m_busAddress = -1;
    int m_deviceAddress = -1;

    bool m_valid = false;
    QString m_errorMessage;

    std::unique_ptr<IAS5600Sensor> m_backend;
    QTimer m_timer;
    void createBackend();
};

#endif // AS5600_H
