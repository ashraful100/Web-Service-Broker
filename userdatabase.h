#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "amqpclient/communicator/communicator.h"

#include "dbwriter.h"
#include "usersdetails.h"

#include <QByteArray>


namespace Ui {
class UserDatabase;
}

class UserDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit UserDatabase(QWidget *parent = 0);
    ~UserDatabase();

signals:
    void sendTransmitterKey(QString key);
    void connectionStatus(bool);
public slots:
    void userListUpdate(QMap<QString, UserInfo> list);
private slots:
//     void on_pbConnect_clicked();

     void on_pbImport_clicked();

     void on_pbStore_clicked();

     void receiveServerUrl(int id,QString url_os,QString url_ts);

     void on_usersList_activated(int index);

     void on_pb_StopStore_clicked();

     void displayMsg(QByteArray array);

     void on_pbSend_clicked();

     void on_pbStoreDummy_clicked();    

     void on_pbStoreDummy_2_clicked();

private:
    Ui::UserDatabase *ui;   
    DbWriter *dbWriter;
    bool open=false;
    QString osServerUrl,tsServerUrl;
    bool setServer= false;   
    Communicator *_communicatorRcvOwnShip;
    Communicator *_communicatorRcvTrafficShip;
    Communicator *_communicatorSend;
    QMap<QString, UserInfo> userList;

    QString currentUserId;
    QString currentUserName;

};

#endif // USERDATABASE_H
