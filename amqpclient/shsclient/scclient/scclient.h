#ifndef SCCLIENT_H
#define SCCLIENT_H

#include <QObject>

#include "amqpclient/receiver.h"
#include "amqpclient/transmitter.h"

#include "amqpclient/shsclient/readais.h"
#include "view/datarategraph/connectionstatusmanager.h"
#include "amqpclient/shsclient/dataviewer.h"

class ScClient : public QObject
{
    Q_OBJECT
public:
    explicit ScClient(QObject *parent = 0);
    ~ScClient();


signals:

    void signalAisMessageReceived(QByteArray);

public slots:

    void connectClient();
    void disconnectClient();
    void resetClient();
    void setAmqpServer(QString url, QString name);
private slots:
    void updateConnectionSettings();
    void createClients();
private:

    QAMQP::Receiver *_receiveAis;
    QString amqpUrl;
    QString qAis;
    QString foAis;

    ReadAis *_readAis;

    bool connectionStatus;
    QString appId;
    ConnectionStatusManager *_conStatusManager;
    QString server;
    int systemId;
    int shipId;
};

#endif // SCCLIENT_H
