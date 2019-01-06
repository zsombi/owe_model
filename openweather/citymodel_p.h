#ifndef CITYMODEL_H
#define CITYMODEL_H

#include <QAbstractListModel>

class CityModelPrivate;
class CityModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    enum Roles
    {
        CityNameRole = Qt::UserRole + 1,
        RegionRole,
        CountryCodeRole
    };
    explicit CityModel(QObject *parent = nullptr);
    ~CityModel();

    // Roles:
    QHash<int, QByteArray> roleNames() const override;
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void filterChanged();
    void error(QString message);

private:

    QString filter() const;
    void setFilter(const QString& filter);

    friend class CityModelPrivate;
    CityModelPrivate* d;
};

#endif // CITYMODEL_H
