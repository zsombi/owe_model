#include "openweatherforecast_p.h"
#include <QJsonArray>
#include <QDateTime>

OpenWeatherForecast::OpenWeatherForecast(QObject *parent)
    : QAbstractListModel(parent)
{
}

OpenWeatherForecast::~OpenWeatherForecast()
{
    if (m_data)
    {
        m_data->unsubscribe(*this);
    }
}

QString OpenWeatherForecast::place() const
{
    return m_data ? m_data->place() : QString();
}
void OpenWeatherForecast::setPlace(const QString& place)
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
    m_data = OpenWeatherCache::get().subscribeForWeatherForecast(*this, place, OpenWeather::Metric);
    m_data->refresh();
    emit placeChanged();
}

void OpenWeatherForecast::onStatusChanged(WeatherData::Status status)
{
    if (status == WeatherData::Ready)
    {
        beginResetModel();
        endResetModel();
    }
    else if (status == WeatherData::Error)
    {
        qDebug() << "FORECAST ERROR!!";
    }
}


QHash<int, QByteArray> OpenWeatherForecast::roleNames() const
{
    // FIXME: Implement me!
    QHash<int, QByteArray> roles;
    roles.insert(DateRole, "day");
    roles.insert(TimeRole, "time");
    roles.insert(IconRole, "weatherIcon");
    roles.insert(DescriptionRole, "description");
    roles.insert(TemperatureRole, "temperature");
    roles.insert(MinTemRole, "minTemperature");
    roles.insert(MaxTempRole, "maxTemperature");
    roles.insert(PressureRole, "pressure");
    roles.insert(HumidityRole, "humidity");
    roles.insert(WindSpeedRole, "windSpeed");
    roles.insert(WindGustRole, "windGust");
    roles.insert(WindDirectionRole, "windDirection");
    return roles;
}

int OpenWeatherForecast::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || m_data->status() != WeatherData::Ready)
    {
        return 0;
    }

    // FIXME: Implement me!
    return m_data->data()["cnt"].toInt();
}

QVariant OpenWeatherForecast::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || m_data->status() != WeatherData::Ready)
    {
        return QVariant();
    }

    // FIXME: Implement me!
    QJsonObject object = m_data->data()["list"].toArray()[index.row()].toObject();
    switch (role)
    {
        case DateRole:
        {
            uint dt = static_cast<uint>(object["dt"].toInt());
            return QDateTime::fromTime_t(dt).date().toString();
        }
        case TimeRole:
        {
            uint dt = static_cast<uint>(object["dt"].toInt());
            return QDateTime::fromTime_t(dt).time().toString();
        }
        case IconRole:
            return owe::weatherIconUrl(object);
        case DescriptionRole:
            return owe::weatherDescription(object);
        case TemperatureRole:
            return owe::mainTemperature(object);
        case MinTemRole:
            return owe::mainMinTemperature(object);
        case MaxTempRole:
            return owe::mainMaxTemperature(object);
        case PressureRole:
            return owe::mainPressure(object);
        case HumidityRole:
            return owe::mainHumidity(object);
        case WindSpeedRole:
            return owe::windSpeed(object);
        case WindGustRole:
            return owe::windGust(object);
        case WindDirectionRole:
            return owe::windDirection(object);
        default:
            return QVariant();
    }
}
