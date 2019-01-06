#include "openweathercache_p_p.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace
{

QString fetchingUrl(bool forecast, const QString& place, OpenWeather::Units units)
{
    QString url(QStringLiteral("http://api.openweathermap.org/data/2.5/%1?q=")
                .arg(forecast ? "forecast" : "weather"));
    url += place;
    switch (units)
    {
        case OpenWeather::Metric:
            url += "&units=metric";
            break;
        case OpenWeather::Imperial:
            url += "&units=imperial";
            break;
        default:
            break;
    }
    url += "&appid=" + OpenWeather::apiKey();
    return url;
}

} // namespace noname

namespace owe
{
QString weatherIconUrl(QJsonObject obj)
{
    QString id = obj["weather"].toArray()[0].toObject()["icon"].toString();
    return QStringLiteral("http://openweathermap.org/img/w/%1.png").arg(id);
}

QString weatherDescription(QJsonObject obj)
{
    return obj["weather"].toArray()[0].toObject()["description"].toString();
}

int mainTemperature(QJsonObject obj)
{
    return qRound(obj["main"].toObject()["temp"].toDouble());
}

int mainMinTemperature(QJsonObject obj)
{
    return qRound(obj["main"].toObject()["temp_min"].toDouble());
}

int mainMaxTemperature(QJsonObject obj)
{
    return qRound(obj["main"].toObject()["temp_max"].toDouble());
}

double mainPressure(QJsonObject obj)
{
    return obj["main"].toObject()["pressure"].toDouble();
}

double mainHumidity(QJsonObject obj)
{
    return obj["main"].toObject()["humidity"].toDouble();
}

double windSpeed(QJsonObject obj)
{
    return obj["wind"].toObject()["speed"].toDouble();
}

double windGust(QJsonObject obj)
{
    return obj["wind"].toObject()["gust"].toDouble();
}

double windDirection(QJsonObject obj)
{
    return obj["wind"].toObject()["deg"].toDouble();
}
} // namespace owe

WeatherData::WeatherData(Type type, SharedNetworkManagerPtr netMan, const QString& place, OpenWeather::Units unit, QObject* parent)
    : QObject(parent)
    , m_netManager(netMan)
    , m_place(place)
    , m_unit(unit)
    , m_status(Ready)
    , m_type(type)
{
    connect(m_netManager.get(), &QNetworkAccessManager::finished, this, &WeatherData::onRequestFinished);
}

WeatherData::~WeatherData()
{
    if (m_status == Fetching)
    {
        m_reply->close();
    }
}

void WeatherData::refresh()
{
    if (m_status == Fetching)
    {
        return;
    }
    if (m_status == Ready && QDateTime::currentDateTime() < m_lastRefresh.addSecs(600))
    {
        // Emit status change to refresh data.
        emit statusChanged(m_status);
    }

    setStatus(Fetching);
    QNetworkRequest request;
    request.setUrl(QUrl(fetchingUrl(m_type == Forecast, m_place, m_unit)));
    m_reply = m_netManager->get(request);
}

void WeatherData::setStatus(Status status)
{
    if (m_status != status)
    {
        m_status = status;
        emit statusChanged(m_status);
    }
}

void WeatherData::onRequestFinished(QNetworkReply* reply)
{
    if (reply != m_reply)
    {
        return;
    }

    if (reply->error() != QNetworkReply::NoError)
    {
        setStatus(Error);
    }
    else
    {
        m_data = QJsonDocument::fromJson(reply->readAll()).object();
        m_lastRefresh = QDateTime::currentDateTime();
        setStatus(Ready);
        // TODO: start timer to auto-refresh.
    }

    reply->deleteLater();
    m_reply = nullptr;
}

bool WeatherData::subscribeClient(QObject* client)
{
    if (!m_clientList.contains(client))
    {
        m_clientList.append(client);
        return true;
    }
    return false;
}

bool WeatherData::unsubscribeClient(QObject* client)
{
    int index = m_clientList.indexOf(client);
    if (index >= 0)
    {
        m_clientList.removeAt(index);
    }
    return (index >= 0);
}

OpenWeatherCache::OpenWeatherCache()
    : m_netManager(new QNetworkAccessManager)
{
}

OpenWeatherCache::~OpenWeatherCache()
{
}
