#ifndef STMCONNECTOR_H
#define STMCONNECTOR_H

#include <QObject>
#include "amqpclient/receiver.h"
#include "amqpclient/transmitter.h"
#include "vis.h"

class StmConnector : public QObject
{
    Q_OBJECT
public:
    explicit StmConnector(QString AppId, QObject *parent = 0);
    ~StmConnector();


signals:

    void signalSendVoMessage(QByteArray);
    void signalOnVoMessageReceived(QByteArray);
    void signalConnectionStatusReceiver(bool);
    void signalConnectionStatusTransmitter(bool);
    void postSingle(int mmsi);
public slots:

    void setConnectionIdentifiers(int shipId, int systemId);
    void connectClient();
    void disconnectClient();
    void resetClient();
    void purgeAllQueues();
    void setAmqpServer(QString url, QString name);

private slots:
    void updateConnectionSettings();
    void createReceivers(QAMQP::Receiver **_receiver, QString &q, QString &fo);
    void createTransmitters(QAMQP::Transmitter **_transmitter, QString &fo);
    void createClients();
private:

    QAMQP::Receiver *_receiver;
    QAMQP::Transmitter *_transmitter;

    int systemId,shipId;

    QString transmitterKey,receiverKey;
    bool connectionStatus;

    QString amqpUrl;
    QString foVO;
    QString  qVO;

    QString appId;
    QString server;

    VIS vis;
};

#endif // STMCONNECTOR_H
