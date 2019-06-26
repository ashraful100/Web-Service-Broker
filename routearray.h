#ifndef ROUTEARRAY_H
#define ROUTEARRAY_H

#include <QDialog>
#include <QDebug>

#include <QMessageBox>
#include <QDateTime>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <iostream>

#include "amqpclient/communicator/communicator.h"
#include "usersdetails.h"

using namespace std;

namespace Ui {
class RouteArray;
}

class RouteArray : public QDialog
{
    Q_OBJECT

signals:
    void serverSettings(QString,QString);
    void sendDataToCommunicator(QByteArray doc);
    void passTransmitterKey(QString transmitterKey );

public:
    explicit RouteArray(QWidget *parent = 0);    
    ~RouteArray();

public slots:
    void userListUpdate(QMap<QString, UserInfo>list);

private slots:
    void on_AddWP_clicked();

    void on_RemoveWP_clicked();

    void on_SendRouteMap_clicked();

     void on_usersList_activated(int index);

private:
    Ui::RouteArray *ui;
    int i;
    QMap<QString, UserInfo> userList;
    bool enlisted=1;
    int indexOfCombobox;
    QString currentUserId;
    QString currentUserName;
    Communicator *_communicator;

};

#endif // ROUTEARRAY_H
