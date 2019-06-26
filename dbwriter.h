#ifndef DBWRITER_H
#define DBWRITER_H

#include <QObject>
#include <QSqlDatabase>
#include <QTime>
#include <QSqlQuery>
#include <QtCore>
#include <QString>
#include <QDebug>
#include <QSqlError>
#include <QByteArray>
#include <QMap>
#include <QSettings>
#include <QGeoCoordinate>

struct dbData{
    int node_id;
    qint64 timestamp;
    QDateTime date_time;
    double lat_ais;
    double lon_ais;
    double lat_reach;
    double lon_reach;
    QMap<int,double> sensors;
    double course;
    int device_id;
};

struct SensorData {
    int device_id;
    int sensor_id;
    QString sensor_name;
    QString sensor_unit;
};

struct trafficShipData{
    double heading;
    double latitude;
    double longitude;
    QString mmsi;
    QString time_Stamp;
};

class DbWriter : public QObject
{
    Q_OBJECT

public:
    explicit DbWriter(QObject *parent = 0);
    bool openDB(int id, QString osUrl, QString tsUrl);
    void closeDB();
    void importDataFromServer();
    void selectDB();
    void createOwnShipDataJsonForTesting();
    void createTrafficShipDataJsonForTesting();
    ~DbWriter();
    QSqlDatabase ownShip_db;
    QSqlDatabase trafficShip_db;
    bool opened=false;

signals:
    void dummyOwnShipJsonForTesting(QByteArray json);
    void dummyTrafficShipJsonForTesting(QByteArray json);
    void sendDecodedSensorData(dbData);
    void sendJsonToServer(QByteArray);    

public slots:
    void receivedOwnShipFromServer(QByteArray array);
    void receivedTrafficShipFromServer(QByteArray array);
    void storeDataIntoDatabase(dbData object);
    void storeTrafficDataIntoDatabase(trafficShipData data);

private:
    QMap<int,QString>listOfDevices;
    QList<SensorData>sensorDb;
    void readListOfDevices();
    void writeIntoDevicesDatabase(int id, QString name);
    void writeIntoSensorDatabase(QList<SensorData>sensors);
    QString tableName;
    QString os_Url,ts_Url;
    int db_ID;
    QList<int>deviceId;
    QMap<int,SensorData>_sensorConfig;
    int size=0;
};

#endif // DBWRITER_H
