#include "stmconnector.h"

StmConnector::StmConnector(QString AppId,QObject *parent) : QObject(parent)
  ,appId(AppId)
{
    shipId =1;
    systemId =1;

    transmitterKey = "TO_VIS";
    receiverKey = "TO_SCC";
    connectionStatus =false;

    //Test only VIS

    connect(this,SIGNAL(signalSendVoMessage(QByteArray)),&vis,SLOT(slotRequestVoyage(QByteArray)));
    connect(&vis,SIGNAL(signalVoyageFromVis(QByteArray)),this,SIGNAL(signalOnVoMessageReceived(QByteArray)));

    connect(&vis,SIGNAL(signalPostVoyageToVis(QByteArray)),this,SIGNAL(signalSendVoMessage(QByteArray)));
    connect(this,SIGNAL(postSingle(int)),&vis,SLOT(postSingle(int)));
}

StmConnector::~StmConnector()
{

}

void StmConnector::setConnectionIdentifiers(int shipId,int systemId){

    this->shipId =shipId;
    this->systemId =systemId;

}

void StmConnector::connectClient(){

    disconnectClient();
    updateConnectionSettings();
    createClients();
    connectionStatus =true;


}


void StmConnector::disconnectClient()
{
    if(connectionStatus)
    {
        _receiver->closeConnection();
        _transmitter->closeConnection();

        _receiver->deleteLater();
        _transmitter->deleteLater();

        connectionStatus =false;

    }
}

void StmConnector::resetClient(){

    disconnectClient();
    connectClient();

    QTimer::singleShot(2000,&vis,SLOT(postToVIS()));
}

void StmConnector::updateConnectionSettings()
{
    if(!connectionStatus)
    {
        qVO = QString("Q_%1_STM_VO_SHIP%2_SYS%3").arg(appId).arg(shipId).arg(systemId);
        foVO =QString("VOYAGE_EXCHANGE");

    }

}

void StmConnector::createClients()
{
    createReceivers(&_receiver,qVO,foVO);
    createTransmitters(&_transmitter,foVO);

    connect(_receiver,SIGNAL(onMessageReceived(QByteArray)),this,SIGNAL(signalOnVoMessageReceived(QByteArray)));
    connect(this,SIGNAL(signalSendVoMessage(QByteArray)),_transmitter,SLOT(sendMessageArray(QByteArray)));
    connect(_receiver,SIGNAL(signalConnectionStatus(bool)),this,SIGNAL(signalConnectionStatusReceiver(bool)));
    connect(_transmitter,SIGNAL(signalConnectionStatus(bool)),this,SIGNAL(signalConnectionStatusTransmitter(bool)));
}


void StmConnector::createReceivers( QAMQP::Receiver **_receiver,QString &q,QString &fo)
{
    (*_receiver) = new QAMQP::Receiver(this);
    (*_receiver)->setQueueName(q,receiverKey);
    (*_receiver)->setFanoutName(fo);
    (*_receiver)->setServerAddress(amqpUrl);
    (*_receiver)->updateSettings();
}


void StmConnector::createTransmitters( QAMQP::Transmitter **_transmitter,QString &fo)
{
    (*_transmitter) = new QAMQP::Transmitter(this);
    (*_transmitter)->setMsgKey(transmitterKey);
    (*_transmitter)->setFanoutName(fo,"direct");
    (*_transmitter)->setFanoutFlag(true);
    (*_transmitter)->setServerAddress(amqpUrl);
    (*_transmitter)->updateSettings();
}

void StmConnector::purgeAllQueues()
{
    if(connectionStatus)
    {
       _receiver->purgeQ();
    }
}

void StmConnector::setAmqpServer(QString url, QString name)
{
    qDebug() << "Here" <<url ;
    amqpUrl = url;
    server = name;
    resetClient();
}
