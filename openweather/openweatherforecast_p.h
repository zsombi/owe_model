#ifndef OPENWEATHERFORECAST_H
#define OPENWEATHERFORECAST_H

#include <QAbstractListModel>
#include "openweathercache_p_p.h"

class OpenWeatherForecast : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString place READ place WRITE setPlace NOTIFY placeChanged)

public:
    enum Roles
    {
        DateRole = Qt::UserRole + 1,    // role: day
        TimeRole,                       // role: time
        IconRole,                       // role: weatherIcon
        DescriptionRole,                // role: description
        TemperatureRole,                // role: temperature
        MinTemRole,                     // role: minTemperature
        MaxTempRole,                    // role: maxTemperature
        PressureRole,                   // role: pressure
        HumidityRole,                   // role: humidity
        WindSpeedRole,                  // role: windSpeed
        WindGustRole,                   // role: windGust
        WindDirectionRole               // role: windDirection
    };
    explicit OpenWeatherForecast(QObject *parent = nullptr);
    ~OpenWeatherForecast() override;

    // Roles:
    QHash<int, QByteArray> roleNames() const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void onStatusChanged(WeatherData::Status status);

signals:
    void placeChanged();

private:
    QString place() const;
    void setPlace(const QString& place);

    SharedWeatherData m_data;
};

#endif // OPENWEATHERFORECAST_H
