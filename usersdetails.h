#ifndef USERSDETAILS_H
#define USERSDETAILS_H

#include <QWidget>
#include "amqpclient/communicator/communicator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QListWidgetItem>
#include <iostream>
#include <algorithm>
#include <userdata.h>

struct UserInfo{
    QString userId;
    QString dataId;
    QString userName;
    QString appId;
    int reqId;
};

using namespace std;


namespace Ui {
class UsersDetails;
}

class UsersDetails : public QWidget
{
    Q_OBJECT

signals:
    void sendUsersList(QMap<QString, UserInfo>list);
//    void sendJsonToSensorInterface(QByteArray);

public slots:
    void onMessageReceivedFromWeb(QByteArray userDetail);
public:
    explicit UsersDetails(QWidget *parent = 0);
    ~UsersDetails();

private slots:
    void on_usersName_itemActivated(QListWidgetItem *item);

private:
    Ui::UsersDetails *ui;
    Communicator *_communicator;
    vector<QJsonDocument>doc;
    bool data_reqId, data_userName;

    vector<string>user;
    vector<string>id_user;
    vector<string>id_req;
    vector<string>id_app;
    UserData *_userData;   
    QMap<QString,UserInfo> userMap;
    QMap<QString,int>userIdToRowMap;
};

#endif // USERSDETAILS_H
