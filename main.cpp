#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "i2c/as5600sensorfactory.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    auto sensor = AS5600SensorFactory::create();
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("i2c_stmp_demo", "Main");

    return app.exec();
}
