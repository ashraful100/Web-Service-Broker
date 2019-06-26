#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>
#include <QCloseEvent>
#include "amqpclient/receiver.h"
#include "amqpclient/transmitter.h"

#include "amqpclient/shsclient/amqpserversettings.h"



//!
//! \brief The Communicator class handles the data exchange between the individual modules (track pilot, voyage, harsh weather, collision avoidance) and between the ANS and the SOC via the RabbitMQ server.
//! \ingroup communicator
//!
//!
class Communicator : public QObject
{
    Q_OBJECT

public:
    explicit Communicator(QString AppId, QObject *parent = 0);
    QString transmitterKey ;
    ~Communicator();


signals:


    void onTpMsgReceived(QByteArray);
    void onWrMsgReceived(QByteArray);
    void onCaMsgReceived(QByteArray);
    void onVoMsgReceived(QByteArray);

    void sendTpMsg(QByteArray);
    void sendWrMsg(QByteArray);
    void sendCaMsg(QByteArray);
    void sendVoMsg(QByteArray);

    void signalReceiverConnectionStatus(bool);
    void signalTransmitterConnectionStatus(bool);

public slots:   
    void setTransmitterKey(QString key);
    void purgeAllQueues();
    void connectClient();
    void disconnectClient();
    void resetClient();
    void setServerSettingsObj(AmqpServerSettings *_amqpServerSettings);
    void setConnectionIdentifiers(int shipId, int systemId);
    void setAmqpServer(QString url, QString name);

private slots:
    void updateConnectionSettings();
    void createClients();
    void createReceivers(QAMQP::Receiver **_receiver, QString &q, QString &fo);
    void createTransmitters(QAMQP::Transmitter **_transmitter, QString &fo);
    void slotVoyageSendStatus(bool status);
    void slotVoyageReceiveStatus(bool status);

private:
    QString amqpUrl;
    QString foTP;
    QString  qTP;

    // AMQP Receivers

    QAMQP::Receiver *_receiveTP;

    QAMQP::Transmitter *_sendTP;

    bool connectionStatus;

    AmqpServerSettings *_amqpServerSettings;

    QString appId;    


    QString receiverKey;

    QString server;



};

#endif // COMMUNICATOR_H
