#ifndef WEATHERDATA_P_H
#define WEATHERDATA_P_H

#include <QObject>
#include <QJsonObject>
#include "openweathercache_p_p.h"

class OpenWeatherConditions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString place READ place WRITE setPlace NOTIFY placeChanged)

    Q_PROPERTY(QString icon READ icon NOTIFY dataChanged)
    Q_PROPERTY(int temperature READ temperature NOTIFY dataChanged)
    Q_PROPERTY(QString description READ description NOTIFY dataChanged)
    Q_PROPERTY(double pressure READ pressure NOTIFY dataChanged)
    Q_PROPERTY(int minTemperature READ minTemperature NOTIFY dataChanged)
    Q_PROPERTY(int maxTemperature READ maxTemperature NOTIFY dataChanged)
    Q_PROPERTY(double humidity READ humidity NOTIFY dataChanged)
    Q_PROPERTY(double windSpeed READ windSpeed NOTIFY dataChanged)
    Q_PROPERTY(double windGust READ windGust NOTIFY dataChanged)
    Q_PROPERTY(double windDirection READ windDirection NOTIFY dataChanged)

public:
    explicit OpenWeatherConditions(QObject *parent = nullptr);
    ~OpenWeatherConditions() override;

    void onStatusChanged(WeatherData::Status status);

signals:
    void placeChanged();
    void dataChanged();

public slots:
    void refresh();

private:
    QString place() const;
    void setPlace(const QString& place);

    QString icon() const;
    int temperature() const;
    QString description() const;
    double pressure() const;
    int minTemperature() const;
    int maxTemperature() const;
    double humidity() const;
    double windSpeed() const;
    double windGust() const;
    double windDirection() const;

    SharedWeatherData m_data;
};

#endif // WEATHERDATA_P_H
