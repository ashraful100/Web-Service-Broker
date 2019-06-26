#ifndef REMOTE_H
#define REMOTE_H

#include <QObject>
#include "amqpclient/receiver.h"
#include "amqpclient/transmitter.h"



class Remote : public QObject
{
    Q_OBJECT
public:
    explicit Remote(QString _appId, QObject *parent = 0);
    ~Remote();

signals:
    void signalSendMsgToRemote(QByteArray msg);
    void signalOnRemoteMessageReceived(QByteArray remoteMsg);
public slots:

    void setAmqpServer(QString url, QString name);
    void disconnectClient();
    void connectClient();
    void resetClient();
private:

    QAMQP::Transmitter *_sendControl;
    QAMQP::Receiver *_receiveControl;

    QString amqpUrl,server;
    bool connectionStatus;
    QString appId;
};

#endif // REMOTE_H
