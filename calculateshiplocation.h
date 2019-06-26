#ifndef CALCULATESHIPLOCATION_H
#define CALCULATESHIPLOCATION_H

#include <QDialog>
#include <QGeoCoordinate>
#include <QDebug>

#include <QMessageBox>
#include <QTimer>
#include <QTextEdit>
#include <QPushButton>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <iostream>

#include "usersdetails.h"

using namespace std;

namespace Ui {
class CalculateShipLocation;
}

class CalculateShipLocation : public QDialog
{
    Q_OBJECT

public:
    explicit CalculateShipLocation(QWidget *parent = 0);

//    void calculatePosition(QString shipID);

    ~CalculateShipLocation();    

    void setId(int value);
    QMap<QString,QString> displayAis;

signals:
    void sendDataToCommunicator(QByteArray doc);

    void sendTransmitterKey(QString);

public slots:
    void userListUpdate(QMap<QString, UserInfo> list);

    void dataUpdate();

    void displayJson(QByteArray array);

    void displayAngle(double angle);  

    void dataUpdate(QMap<QString, QString> decodedData);
private slots:
    void on_calculate_2_clicked();

    void on_sendPosition_clicked();

    void on_stopSending_clicked();     

//    void on_usersList_currentIndexChanged(const QString &arg1);

    void on_usersList_activated(int index);

private:
    double Lat,Lon,Angle,Speed,Time;
    QString Name, Destination;
    QTimer *timer;
    QString publisher, subscriber;
    Ui::CalculateShipLocation *ui;
    QJsonObject shipObj;
    QJsonArray shipArray;
    QJsonDocument shipDoc;
    int id,routeId=1;
    QString transmitterKey;
    QMap<QString, UserInfo> userList; //<userId,userInfo>
    bool enlisted=1;
    int indexOfCombobox;

    QString currentUserId;
    QString currentUserName;
};

#endif // CALCULATESHIPLOCATION_H
