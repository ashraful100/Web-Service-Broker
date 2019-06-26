#ifndef SHIPDATA_H
#define SHIPDATA_H

#include <QObject>
#include <QSettings>
#include <QMap>
#include <QtCore>

class ShipData : public QObject
{
    Q_OBJECT
public:
    explicit ShipData(QObject *parent = 0);
    ~ShipData();

    QMap<double, int> getEotMap() const;
    void setEotMap(const QMap<double, int> &value);

    QMap<int, double> getSogMap() const;
    void setSogMap(const QMap<int, double> &value);

    QString getDefaultShipName() const;
    void setDefaultShipName(const QString &value);

signals:
    void signalSendSpeedMaps(QMap<double , int> eotMap, QMap<int , double> sogMap);
    void signalSpeedAtMaxEot(double speedAtMaxEot);
public slots:
    void readSettings();
    void writeSettings();
    void broadcastSpeedMaps();
    void loadSettingsForShip(QString shipName);
private:
    QString currentShipName;
     QString defaultShipName;
    QMap<double, int > eotMap,eotMapDefault;
    QMap<int, double > sogMap;
};

#endif // SHIPDATA_H
