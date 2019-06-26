#include "availabilitycheck.h"

//! \class Availability
//! \brief The Availability class checks whether a connection to a ship is available at the moment or not.
//! \ingroup shsclient
//!

Availability::Availability(QString AppId, int shsId,QWidget *parent) :
    QMainWindow(parent),appId(AppId),shsId(shsId)

{
    listShsFanoutName << "FO_ANS_SCC_1"<< "FO_ANS_SCC_2"<< "FO_ANS_SCC_3"<< "FO_ANS_SCC_4"<< "FO_ANS_SCC_5"<< "FO_ANS_SCC_6";

    connectionStatus =false;

    _xmlParser =new XmlParser(this);
    connect(_xmlParser,SIGNAL(signalShipName(QString&)),this,SLOT(slotXmlShipData(QString&)));

    dataStreamTimer =new QTimer();
    connect(dataStreamTimer,SIGNAL(timeout()),this,SLOT(dataTimeOut()));
    dataStreamTimer->start(7000);

    reconnectTimer  =new QTimer();
    connect(reconnectTimer,SIGNAL(timeout()),this,SLOT(attemptReconnect()));
    reconnectTimer->start(3000);


    source =1;
    //AIS Decoding for availability check
//    _aisDecoder =new AISDecoderLib(shsId);
//    _aisDecoder->setSource(shsId);
//     connect(_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*,int)),this,SLOT(slotAisShipData(EcAISTargetInfo*,int)));


}

Availability::~Availability()
{
    delete dataStreamTimer;
    delete _receive;
   // delete _receiveAis;
    delete _xmlParser;
    delete reconnectTimer;
}

void Availability::connectClient(){

    if (amqpUrl.isEmpty()) {
        timer.singleShot(1000,this,SLOT(connectClient()));
        return;
    }

    updateConnectionSettings();
    createClients();
    connectionStatus =true;
}

void Availability::disconnectClient()
{
    if(connectionStatus)
    {

        _receive->closeConnection();
        _receive->slotDisconnected();
        delete _receive;

//        _receiveAis->closeConnection();
//        _receiveAis->slotDisconnected();
//        delete _receiveAis;

        dataStreamTimer->stop();
        reconnectTimer->stop();
        dataTimeOut();
        connectionStatus =false;
    }
}

void Availability::resetClient(){

    disconnectClient();
    connectClient();
}


void Availability::updateConnectionSettings()
{
    if(!connectionStatus)
    {
        queue = QString("Q_%1_AVC_SHIP%2").arg(appId).arg(shsId);
        fanout = listShsFanoutName.value(shsId-1);
        qAis = QString("Q_%1_AVC_AIS_SHIP%2").arg(appId).arg(shsId);
        foAis = QString("FO_NMEA_AIS_%1").arg(shsId);

    }
}


void Availability::createClients()
{

    _receive = new QAMQP::Receiver();
    _receive->setQueueName(queue);
    _receive->setFanoutName(fanout);
    _receive->setServerAddress(amqpUrl);
    _receive->updateSettings();
    connect(_receive,SIGNAL(onMessageReceived(QByteArray)),_xmlParser,SLOT(parseXML(QByteArray)));

//    _receiveAis = new QAMQP::Receiver();
//    _receiveAis->setQueueName(qAis);
//    _receiveAis->setFanoutName(foAis);
//    _receiveAis->setServerAddress(amqpUrl);
//    _receiveAis->updateSettings();
   // connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_xmlParser,SLOT(parseXML(QByteArray)));
    //connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_aisDecoder,SIGNAL(signalInputAisNmeaData(QByteArray)));


}

void Availability::purgeAllQueues()
{
    if(connectionStatus)
    {
        _receive->purgeQ();
//        _receiveAis->purgeQ();
    }
}

//void Availability::slotAisShipData(EcAISTargetInfo *ti,int shsId){

//    QString name = ti->shipName;
//    qDebug() << name;
//    qDebug() << shsId;
//    qDebug() << this->shsId << "AVC ID";

//    if(this->shsId!=shsId)
//        return;

//    if(!ti->ownShip)
//        return;



//    if(source==3)
//    slotShipDataOnAvailable(name);

//}


void Availability::slotXmlShipData(QString &shipName)
{
    if(source==1)
    slotShipDataOnAvailable(shipName);
}

void Availability::slotShipDataOnAvailable(QString &shipName)
{

    emit signalDataAvailable(shsId,1,dataInfo,shipName);
    dataStreamTimer->start(10000);
    reconnectTimer->start(3000);
}

void Availability::setServerSettingsObj( AmqpServerSettings *_amqpServerSettings){

    this->_amqpServerSettings =_amqpServerSettings;
}

void Availability::dataTimeOut()
{
    QString name ="";
    emit signalDataAvailable(shsId,0,dataInfo,name);
    dataStreamTimer->stop();

}

void Availability::attemptReconnect()
{
    QString name ="";
    emit signalDataAvailable(shsId,2,dataInfo,name);
    reconnectTimer->stop();
}

void Availability::setAmqpServer(QString url, QString name)
{
    amqpUrl = url;
    serverName =name;
    resetClient();
}

void Availability::slotOnOsDataSourceChanged(int sourceId)
{
    this->source =sourceId;

    dataInfo = QString("%1;%2").arg(serverName).arg(source);
}

