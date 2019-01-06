#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "openweather/weatherapi.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("edu");
    QCoreApplication::setApplicationName("weather");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    OpenWeatherModule::registerQmlTypes(engine, "edu.openweather");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
