#ifndef DBREADER_H
#define DBREADER_H

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
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <iostream>

#include<amqpclient/communicator/communicator.h>

using namespace std;

class DbReader : public QObject
{
    Q_OBJECT

signals:
    void sendSensorDataToWebInterface(QByteArray);

public:
    explicit DbReader(QString osUrl, QString tsUrl,QString userId, QObject *parent = nullptr);

    bool openDB(QString osUrl, QString tsUrl);
    void closeDB();    

    bool opened=false;
    void checkDataId(QByteArray array);
    void setCommunicatorSettings(QString url, QString name);

signals:

public slots:

private slots:
    QJsonObject getRequestComplete();
private:
    void sendDeviceList();
    void sendSensorList(QByteArray array);
    void readSensorListFromConfig(QList<int>id);
    void readSensorListFromDb(QList<int>id);
    void sendAllDetails(QByteArray array);
    void sendTrafficShips(QByteArray array);
    void sendBunchOfData(QByteArray ary, int deviceId);
    QSqlDatabase os_db, ts_db;
    Communicator *_communicator;   

};

#endif // DBREADER_H
