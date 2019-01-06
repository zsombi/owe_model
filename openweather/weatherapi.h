#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <QObject>
#include <QNetworkReply>
#include <QQmlEngine>
#include <QStringListModel>

class WeatherData;
typedef std::shared_ptr<WeatherData> SharedWeatherData;

class OpenWeather : public QObject
{
    Q_OBJECT

public:
    enum Units
    {
        Metric,
        Imperial,
        US
    };
    Q_ENUM(Units)

    Q_PROPERTY(Units units MEMBER m_units NOTIFY unitsChanged)
    Q_PROPERTY(QString temperatureSymbol READ tempSymbol NOTIFY unitsChanged)
    Q_PROPERTY(QStringList cityModel READ cityModel NOTIFY modelChanged)
    Q_PROPERTY(int lastCityIndex MEMBER m_lastCityIndex NOTIFY lastCityIndexChanged)

    OpenWeather();
    ~OpenWeather() override;
    static Units units();
    static void setApiKey(const QString& key)
    {
        m_apiKey = key;
    }
    static QString apiKey()
    {
        return m_apiKey;
    }

public slots:
    void addCity(const QString& city);
    void removeCity(int index);

signals:
    void unitsChanged();
    void modelChanged();
    void lastCityIndexChanged();

private:
    static QString m_apiKey;
    QStringList m_cityModel;
    Units m_units{Metric};
    int m_lastCityIndex{-1};
    static QPointer<OpenWeather> m_instance;

    QString tempSymbol() const;
    QStringList cityModel() const
    {
        return m_cityModel;
    }

    void saveStatus();
};

namespace OpenWeatherModule
{

void registerQmlTypes(QQmlEngine& engine, const char* uri);

}

#endif // WEATHERAPI_H
