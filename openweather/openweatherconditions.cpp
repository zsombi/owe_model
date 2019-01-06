#include "openweatherconditions_p.h"
#include "openweathercache_p_p.h"
#include <QJsonArray>

QString OpenWeatherConditions::place() const
{
    return m_data ? m_data->place() : QString();
}
void OpenWeatherConditions::setPlace(const QString& place)
{
    if ((!m_data && place.isEmpty()) || (m_data && m_data->place() == place))
    {
        return;
    }
    if (m_data)
    {
        m_data->unsubscribe(*this);
        m_data.reset();
    }
    m_data = OpenWeatherCache::get().subscribeForCurrentWeather(*this, place, OpenWeather::units());
    emit placeChanged();

    refresh();
}

void OpenWeatherConditions::onStatusChanged(WeatherData::Status status)
{
    Q_UNUSED(status);
    if (status == WeatherData::Ready)
    {
        emit dataChanged();
    }
}

OpenWeatherConditions::OpenWeatherConditions(QObject *parent)
    : QObject(parent)
{
}
OpenWeatherConditions::~OpenWeatherConditions()
{
    if (m_data)
    {
        m_data->unsubscribe(*this);
    }
}

void OpenWeatherConditions::refresh()
{
    if (m_data)
    {
        m_data->refresh();
    }
}

QString OpenWeatherConditions::icon() const
{
    return !m_data ? QString() : owe::weatherIconUrl(m_data->data());
}

int OpenWeatherConditions::temperature() const
{
    return!m_data
            ? 0
            : owe::mainTemperature(m_data->data());
}

QString OpenWeatherConditions::description() const
{
    return !m_data
            ? QString()
            : owe::weatherDescription(m_data->data());
}

double OpenWeatherConditions::pressure() const
{
    return !m_data
            ? 0.0
            : owe::mainPressure(m_data->data());
}

int OpenWeatherConditions::minTemperature() const
{
    return !m_data
            ? 0
            : owe::mainMinTemperature(m_data->data());
}

int OpenWeatherConditions::maxTemperature() const
{
    return !m_data
            ? 0
            : owe::mainMaxTemperature(m_data->data());
}

double OpenWeatherConditions::humidity() const
{
    return !m_data
            ? 0.0
            : owe::mainHumidity(m_data->data());
}

double OpenWeatherConditions::windSpeed() const
{
    return !m_data
            ? 0.0
            : owe::windSpeed(m_data->data());
}

double OpenWeatherConditions::windGust() const
{
    return !m_data
            ? 0.0
            : owe::windGust(m_data->data());
}

double OpenWeatherConditions::windDirection() const
{
    return !m_data
            ? 0.0
            : owe::windDirection(m_data->data());
}
