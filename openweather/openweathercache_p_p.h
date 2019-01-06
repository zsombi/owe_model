#ifndef OPENWEATHERCACHE_P_P_H
#define OPENWEATHERCACHE_P_P_H

#include <QObject>
#include <memory>

#include <QJsonObject>
#include <QDateTime>
#include <QNetworkAccessManager>
#include "weatherapi.h"

class WeatherData;
typedef std::shared_ptr<WeatherData> SharedWeatherData;
typedef std::shared_ptr<QNetworkAccessManager> SharedNetworkManagerPtr;

class WeatherData : public QObject
{
    Q_OBJECT

public:
    enum Status
    {
        Expired,
        Fetching,
        Ready,
        Error
    };

    enum Type
    {
        CurrentWeather,
        Forecast
    };

    ~WeatherData() override;

    Type type() const
    {
        return m_type;
    }
    Status status() const
    {
        return m_status;
    }
    QJsonObject data() const
    {
        return m_status == Ready ? m_data : QJsonObject();
    }
    QString place() const
    {
        return m_place;
    }

    void refresh();

    template <class Client>
    void subscribe(Client& client)
    {
        if (subscribeClient(&client))
        {
            connect(this, &WeatherData::statusChanged, &client, &Client::onStatusChanged);
        }
    }

    template <class Client>
    void unsubscribe(Client& client)
    {
        if (unsubscribeClient(&client))
        {
            disconnect(this, &WeatherData::statusChanged, &client, &Client::onStatusChanged);
        }
    }

signals:
    void statusChanged(Status status);

private:
    SharedNetworkManagerPtr m_netManager;
    QDateTime m_lastRefresh;
    QString m_place;
    QJsonObject m_data;
    QList<QObject*> m_clientList;
    QNetworkReply* m_reply{nullptr};
    OpenWeather::Units m_unit;
    Status m_status;
    Type m_type;

    friend class OpenWeatherCache;

    explicit WeatherData(Type type, SharedNetworkManagerPtr netMan, const QString& place, OpenWeather::Units unit, QObject* parent = nullptr);
    void setStatus(Status status);
    Q_SLOT void onRequestFinished(QNetworkReply* reply);
    bool subscribeClient(QObject* client);
    bool unsubscribeClient(QObject* client);

};

class OpenWeatherCache : public QObject
{
    Q_OBJECT
public:
    static OpenWeatherCache& get()
    {
        static OpenWeatherCache staticCache;
        return staticCache;
    }
    ~OpenWeatherCache();

    template <class Client>
    SharedWeatherData subscribeForCurrentWeather(Client& client, const QString& place, OpenWeather::Units units)
    {
        Container::iterator it = cache.find(place);
        if (it == cache.end())
        {
            it = cache.insert(place, WeatherRecord());
        }
        if (!it->currentWeather)
        {
            it->currentWeather = SharedWeatherData(new WeatherData(WeatherData::CurrentWeather, m_netManager, place, units));
        }
        it->currentWeather->subscribe(client);
        return it->currentWeather;
    }

    template <class Client>
    SharedWeatherData subscribeForWeatherForecast(Client& client, const QString& place, OpenWeather::Units units)
    {
        Container::iterator it = cache.find(place);
        if (it == cache.end())
        {
            it = cache.insert(place, WeatherRecord());
        }
        if (!it->forecast)
        {
            it->forecast = SharedWeatherData(new WeatherData(WeatherData::Forecast, m_netManager, place, units));
        }
        it->forecast->subscribe(client);
        return it->forecast;
    }

signals:
    void currentWeatherReady();

private:
    OpenWeatherCache();

    struct WeatherRecord
    {
        SharedWeatherData currentWeather;
        SharedWeatherData forecast;
        WeatherRecord() {}
    };

    typedef QMap<QString, WeatherRecord> Container;

    Container cache;
    SharedNetworkManagerPtr m_netManager;
    QMetaObject::Connection netConnect;
};

namespace owe
{
QString weatherIconUrl(QJsonObject obj);
QString weatherDescription(QJsonObject obj);
int mainTemperature(QJsonObject obj);
int mainMinTemperature(QJsonObject obj);
int mainMaxTemperature(QJsonObject obj);
double mainPressure(QJsonObject obj);
double mainHumidity(QJsonObject obj);
double windSpeed(QJsonObject obj);
double windGust(QJsonObject obj);
double windDirection(QJsonObject obj);
} // namespace owe_json

#endif // OPENWEATHERCACHE_P_P_H
