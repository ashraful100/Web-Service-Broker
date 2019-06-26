#include "scclient.h"

ScClient::ScClient(QObject *parent) : QObject(parent)
{
    shipId =1;
    systemId =1;

    connectionStatus =false;
}

ScClient::~ScClient()
{

}

void ScClient::connectClient(){

    disconnectClient();
    updateConnectionSettings();
    createClients();
    connectionStatus =true;
}

void ScClient::disconnectClient()
{
    if(connectionStatus)
    {
        _receiveAis->closeConnection();
        _receiveAis->deleteLater();
        connectionStatus =false;

    }
}

void ScClient::resetClient(){

    disconnectClient();
    connectClient();
}

void ScClient::updateConnectionSettings()
{
    if(!connectionStatus)
    {
        qAis = QString("Q_%1_AIS_SHIP%2_SYS%3").arg(appId).arg(shipId).arg(systemId);

        foAis = QString("FO_NMEA_AIS_%1").arg(shipId);

         _conStatusManager->connectionStatusWidget(1)->setServerComponents(server,qAis,foAis);

    }

}

void ScClient::createClients()
{



    _receiveAis = new QAMQP::Receiver(this);
    _receiveAis->setQueueName(qAis);
    _receiveAis->setFanoutName(foAis);
    _receiveAis->setServerAddress(amqpUrl);
    _receiveAis->updateSettings();
    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_readAis,SLOT(onDataReceived(QByteArray)),Qt::QueuedConnection);
    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),this,SIGNAL(signalAisMessageReceived(QByteArray)));
    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->dataViewer(1),SLOT(setData(QByteArray)));
    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->connectionStatusWidget(1),SLOT(onDataReceived()));
    connect(_receiveAis,SIGNAL(signalConnectionStatus(bool)),_conStatusManager->connectionStatusWidget(1),SLOT(setConnectionStatus(bool)));



}
void ScClient::setAmqpServer(QString url, QString name)
{
    amqpUrl = url;
    server = name;
    resetClient();
}
