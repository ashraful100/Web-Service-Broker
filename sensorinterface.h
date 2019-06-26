#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#include <QSqlDatabase>
#include <QWidget>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <dbreader.h>

namespace Ui {
class SensorInterface;
}

class SensorInterface : public QWidget
{
    Q_OBJECT

public:
    explicit SensorInterface(QWidget *parent = nullptr);
    ~SensorInterface();

signals:
    void sendSensorData(QByteArray);
    void connectionStatus(bool);   

public slots:
    void checkDataID(QByteArray array);
    void checkReceivedJson(QByteArray array);
    void receiveServerUrl(int id, QString serverUrl);
    void onWebDataRecived(QByteArray data);
    void checkDatabaseConnection(int id, QString url_os, QString tsUrl);
    void setCommunicatorSettings(QString url, QString name);


private:
    Ui::SensorInterface *ui;
    bool setServer=false;
    void setDbReader(DbReader* dbreader , QString userId);
    DbReader *getDbReader(QString userId);
    QMap<QString,DbReader*>dbReaderMap;
    QSqlDatabase ownShip_db,trafficShip_db;
    bool opened=false;
    QString ownShip,trafficShip;
    QString url_Communicator,name_Communicator;
    QString user_id;

};

#endif // SENSORINTERFACE_H
