#ifndef DISPLAYRECEIVEDAISDATA_H
#define DISPLAYRECEIVEDAISDATA_H

#include <QDialog>
#include <QGeoCoordinate>
#include "amqpclient/shsclient/shsclient.h"
#include <aisdecoderlib.h>

#include <usersdetails.h>

namespace Ui {
class DisplayReceivedAISData;
}

class DisplayReceivedAISData : public QDialog
{
    Q_OBJECT

signals:
    void aisData(QByteArray AIS);
    void emitAisData(QByteArray ais);
    void setServerAddress(QString,QString);
    void sendDataToCommunicator(QByteArray);
    void sendTransmitterKey(QString);
    void sendConnectionStatus(bool);

public slots:
    void userListUpdate(QMap<QString, UserInfo>list);

    void receiveID(int id);
    void displayReceivedAis(QByteArray ais);
    void slotOnAisDecodedReceived(QMap<QString, QString> decodedAis);

public:
    explicit DisplayReceivedAISData(QWidget *parent = 0);
    void setShipID(int i);
    ~DisplayReceivedAISData();

private:
    Ui::DisplayReceivedAISData *ui;
    AISDecoderLib *_aisDecoder;
    ShsClient *_shsclient;
    QJsonDocument shipDataDoc;
    QMap<int, QMap<QString, QString> > shipDataMapStatic;
    QMap<int, QMap<QString, QString> > shipDataMapDynamic;
    int displayMMSI;
    QMap<QString, UserInfo> userList;
    bool enlisted=1;
    int indexOfCombobox;

    QString currentUserId;
    QString currentUserName;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_sendAis_clicked();
    void on_stopSendingAis_clicked();
    void sendAisData(QByteArray array);

    //    void on_cbShip_activated(int index);
    void on_pbRefresh_clicked();

    void displayDataInUI();

    void updateStaticData(QMap<QString, QString> data);
    void updateDynamicData(QMap<QString, QString> data);
    void composeJSONfromDecodedAIS(int mmsi);

    void on_usersList_activated(int index);
    void on_receivedAis_textChanged();
};

#endif // DISPLAYRECEIVEDAISDATA_H
