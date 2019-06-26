#ifndef DBCLIENTS_H
#define DBCLIENTS_H

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

struct dbData{
    int node_id;
    double timestamp;
    QString date_time;
    double lat;
    double lon;
    QMap<int,double>sensors;
    double course;
    double device_id;
};

struct SensorData {
    int device_id;
    QString device_name;
    int id;
    QString sensor_name;
};

class DbWriter : public QObject
{
    Q_OBJECT
public:
    explicit DbWriter(QObject *parent = 0);

    bool openDB(QString url);
    void closeDB();
    void importDataFromServer();
    void selectDB();
    void createJsonForTesting();
    void writeIntoDevicesList(double id, QString name);

    QSqlDatabase db;

    bool opened=false;

signals:    
    void dummyJsonForTesting(QByteArray json);
    void sendDecodedSensorData(dbData);
    void sendJsonToServer(QByteArray);


public slots:    
    void receiveJsonFromServer(QByteArray array);
    void storeDataIntoServer(dbData object);

private:
    QMap<double, QString>listOfDevices;
    QList<SensorData>sensorList;
    void readListOfDevices();

};

#endif // DBCLIENTS_H
