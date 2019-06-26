#ifndef MULTISHIPAISDATA_H
#define MULTISHIPAISDATA_H

#include <QDialog>
#include <QButtonGroup>
#include <QDebug>

#include "displayreceivedaisdata.h"
#include "amqpclient/communicator/communicator.h"


namespace Ui {
class MultiShipAISData;
}

class MultiShipAISData : public QDialog
{
    Q_OBJECT

signals:
    void serverSettings(QString,QString);
    void sendServerAddress(QString,QString);
//    void passAisData(QByteArray);
    void passUsersName(QMap<QString, UserInfo>);
    void passTransmitterKey(QString);
    void sendConnectionStatus(bool);

public:
    explicit MultiShipAISData(QWidget *parent = 0);
    ~MultiShipAISData();

private:
    Ui::MultiShipAISData *ui;
    QPushButton *getShipButton(int id);
    QButtonGroup aisButtongroup;    
    Communicator *_communicator;

};

#endif // MULTISHIPAISDATA_H
