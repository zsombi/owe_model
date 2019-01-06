#include "citymodel_p.h"
#include <QMap>
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class CityModelPrivate
{

public:
    struct CityData
    {
        QString name;
        QString region;
        QString countryCode;
        explicit CityData(QString name, QString region, QString countryCode)
            : name(name)
            , region(region)
            , countryCode(countryCode)
        {
        }
    };
    typedef QList<CityData> DataContainer;

    QMetaObject::Connection networkConnection;
    DataContainer cities;
    QString filter;
    QNetworkAccessManager manager;
    CityModel* q_ptr{nullptr};

    explicit CityModelPrivate(CityModel* qq)
        : q_ptr(qq)
    {
        auto lambda = [this](QNetworkReply* reply)
        {
            onRequestFinished(reply);
        };
        networkConnection = QObject::connect(&manager, &QNetworkAccessManager::finished, lambda);

        // Search in English.
        QUrl search("http://www.yr.no/_/settspr.aspx");
        QUrlQuery query;
        query.addQueryItem("spr", "eng");
        query.addQueryItem("redir", "/");
        search.setQuery(query);
        manager.get(QNetworkRequest(search));
    }
    ~CityModelPrivate()
    {
        QObject::disconnect(networkConnection);
    }

    void refresh()
    {
        if (filter.isEmpty())
        {
            if (!cities.isEmpty())
            {
                cities.clear();
                q_ptr->beginResetModel();
                q_ptr->endResetModel();
            }
            return;
        }
        QUrl url("http://www.yr.no/_/websvc/jsonforslagsboks.aspx");
        QUrlQuery query;
        query.addQueryItem("s", filter);
        url.setQuery(query);

        manager.get(QNetworkRequest(url));
        cities.clear();
    }

    void onRequestFinished(QNetworkReply *reply)
    {
        if (!reply)
        {
            return;
        }
        if (reply->request().url().query() != "spr=eng&redir=/")
        {
            if (reply->error() != QNetworkReply::NoError)
            {
                emit q_ptr->error(reply->errorString());
            }
            else
            {
                QString data = reply->readAll();
                QRegExp regExp;
                regExp.setPattern("^\\[\\[.*\\],\\[\\[(.*)\\]\\]\\]$");
                regExp.exactMatch(data);
                QString foundCities = regExp.capturedTexts().at(1);
                foundCities.remove('"');
                QStringList citiesFound = foundCities.split(QRegExp("\\],\\["), QString::SkipEmptyParts);
                for (const QString& s : citiesFound)
                {
                    const QStringList slist = s.split(',');
                    int size = slist.size();
                    cities.append(CityData(slist[0], slist[size - 2], slist[size - 1]));
                }
            }

            q_ptr->beginResetModel();
            q_ptr->endResetModel();
        }
        reply->deleteLater();
    }
};

CityModel::CityModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new CityModelPrivate(this))
{
}
CityModel::~CityModel()
{
    delete d;
}

QString CityModel::filter() const
{
    return d->filter;
}
void CityModel::setFilter(const QString &filter)
{
    if (d->filter != filter)
    {
        d->filter = filter;

        // reftesh data using the filter.
        d->refresh();

        emit filterChanged();
    }
}

QHash<int, QByteArray> CityModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(CityNameRole, "city");
    roles.insert(RegionRole, "region");
    roles.insert(CountryCodeRole, "country");
    return roles;
}

int CityModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
    {
        return 0;
    }

    return d->cities.size();
}

QVariant CityModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case CityNameRole:
        {
            return d->cities.at(index.row()).name;
        }
        case RegionRole:
        {
            return d->cities.at(index.row()).region;
        }
        case CountryCodeRole:
        {
            return d->cities.at(index.row()).countryCode;
        }
        default:
        {
            return QVariant();
        }
    }
}
