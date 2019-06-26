#ifndef MULTISHIPCONTROLLER_H
#define MULTISHIPCONTROLLER_H

#include <QWidget>
#include <calculateshiplocation.h>
#include <QButtonGroup>
#include <QDebug>
#include "amqpclient/communicator/communicator.h"

namespace Ui {
class MultiShipController;
}

class MultiShipController : public QWidget
{
    Q_OBJECT

public:
    explicit MultiShipController(QWidget *parent = 0);
//     Communicator *_communicator;
    ~MultiShipController();

signals:    
    void serverSettings(QString,QString);
    void shipNameSendToServer(QString);
//    void signalSendToServer(QByteArray data);
    void passUsersName(QMap<QString, UserInfo>);

private slots:
    QPushButton *getShipButton(int id);
//    void json(QByteArray doc);

private:
    Ui::MultiShipController *ui;
    QButtonGroup shipButtongroup;
    Communicator *_communicator;




//    QAMQP::Transmitter *transmit;
};

#endif // MULTISHIPCONTROLLER_H
