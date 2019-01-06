#include "weatherapi.h"
#include "citymodel_p.h"
#include "openweatherconditions_p.h"
#include "openweatherforecast_p.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtQml>

namespace
{

QObject* selfRegister(QQmlEngine *, QJSEngine *)
{
    return new OpenWeather;
}

}

namespace OpenWeatherModule
{

void registerQmlTypes(QQmlEngine& /*engine*/, const char* uri)
{
    qmlRegisterType<CityModel>(uri, 1, 0, "CitySearchModel");
    qmlRegisterType<OpenWeatherConditions>(uri, 1, 0, "OpenWeatherConditions");
    qmlRegisterType<OpenWeatherForecast>(uri, 1, 0, "OpenWeatherForecast");

    qmlRegisterSingletonType<OpenWeather>(uri, 1, 0, "OpenWeather", selfRegister);
}

}

QPointer<OpenWeather> OpenWeather::m_instance;
QString OpenWeather::m_apiKey;

OpenWeather::OpenWeather()
{
    m_instance = this;

    QSettings settings;
    m_cityModel = settings.value("cities").toStringList();
    m_lastCityIndex = settings.value("lastCity").toInt();
    m_units = static_cast<Units>(settings.value("units", OpenWeather::Metric).toInt());
    qDebug() << "Cities:" << m_cityModel;
    qDebug() << "LastCity:" << m_lastCityIndex;

    connect(this, &OpenWeather::lastCityIndexChanged, this, &OpenWeather::saveStatus);
    connect(this, &OpenWeather::modelChanged, this, &OpenWeather::saveStatus);
    connect(this, &OpenWeather::unitsChanged, this, &OpenWeather::saveStatus);
}
OpenWeather::~OpenWeather()
{
    m_instance.clear();
}

OpenWeather::Units OpenWeather::units()
{
    return m_instance ? m_instance->m_units : Metric;
}

QString OpenWeather::tempSymbol() const
{
    switch (m_units)
    {
        case Metric:
            return QString("\u2103");
        case Imperial:
            return QString("\u2109");
        case US:
            return QString("\u212A");
    }
}

void OpenWeather::addCity(const QString& city)
{
    m_cityModel.append(city);
    emit modelChanged();
}

void OpenWeather::removeCity(int index)
{
    if (index >= 0 && index < m_cityModel.size())
    {
        m_cityModel.removeAt(index);

        if (index == m_lastCityIndex)
        {
            --index;
            if (index < 0)
            {
                index = 0;
            }
            m_lastCityIndex = index;
            emit lastCityIndexChanged();
        }
        emit modelChanged();
    }
}

void OpenWeather::saveStatus()
{
    qDebug() << "Saving cities:" << m_cityModel;
    qDebug() << "Saving lastCity:" << m_lastCityIndex;

    QSettings settings;
    settings.setValue("cities", m_cityModel);
    settings.setValue("lastCity", m_lastCityIndex);
    settings.setValue("units", m_units);
    settings.sync();
}
