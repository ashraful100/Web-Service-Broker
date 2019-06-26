                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           #include "shsclient.h"

//! \class ShsClient
//! \brief The ShsClient class provides an abstract user interface for communicating with the RabbitMQ server via the qAMQP library.
//! The client receives the Ship data and sends the Ship commands to the RabbitMQ server. The data is identified using ship Ids and system Ids
//! \ingroup shsclient
//!

ShsClient::ShsClient(QString AppId, QString queue, QWidget *parent) :
    QMainWindow(parent),appId(AppId)
{
    shipId =1;
    systemId =1;
    foAis = AppId;
    qAis = queue;

    connectionStatus =false;

//    _readGeneral =new ReadGeneral();
//    _readAis =new ReadAis();
//    _radar =new ReadTtm();
//    _xmlParser =new XmlParser();
//    _nmeaComposer =new NmeaComposer();

//    connect(this,SIGNAL(signalOnOsDataSourceChanged(int)),_readGeneral,SLOT(setOsDataSource(int)));
//    connect(this,SIGNAL(signalOnOsDataSourceChanged(int)),_xmlParser,SLOT(setOsDataSource(int)));
//     connect(_readGeneral,SIGNAL(signalXmlFromNmea(QByteArray)),_xmlParser,SLOT(parseXML(QByteArray)));

//    _conStatusManager->connectionStatusWidget(0)->setDataType("SHS","XML");
//    _conStatusManager->connectionStatusWidget(1)->setDataType("AIS","NMEA");
//    _conStatusManager->connectionStatusWidget(2)->setDataType("TTM","NMEA");
//    _conStatusManager->connectionStatusWidget(3)->setDataType("CMD","NMEA");

//    _conStatusManager->dataViewer(0)->setIdentifier(appId,shipId);
//    _conStatusManager->dataViewer(1)->setIdentifier(appId,shipId);
//    _conStatusManager->dataViewer(2)->setIdentifier(appId,shipId);
//    _conStatusManager->dataViewer(3)->setIdentifier(appId,shipId);

//    _conStatusManager->dataViewer(0)->setDataType("SHS","XML");
//    _conStatusManager->dataViewer(1)->setDataType("AIS","NMEA");
//    _conStatusManager->dataViewer(2)->setDataType("TTM","NMEA");
//    _conStatusManager->dataViewer(3)->setDataType("CMD","NMEA");

//    connect(_nmeaComposer,SIGNAL(sendControlMsg(QString)),_conStatusManager->dataViewer(3),SLOT(setDataStr(QString)));
//    connect(_nmeaComposer,SIGNAL(sendControlMsg(QString)),_conStatusManager->connectionStatusWidget(3),SLOT(onDataReceived()));
}

ShsClient::~ShsClient()
{
//    delete _radar;
}

void ShsClient::setConnectionIdentifiers(int shipId,int systemId){

    this->shipId =shipId;
    this->systemId =systemId;
    resetClient();
}

void ShsClient::connectClient(){    

    disconnectClient();
    updateConnectionSettings();
    createClients();
    connectionStatus =true;
}

void ShsClient::disconnectClient()
{
    if(connectionStatus)
    {
//        _receiveShs->closeConnection();
        _receiveAis->closeConnection();
//        _receiveTtm->closeConnection();
//        _receiveNav->closeConnection();

//        _sendCmd->closeConnection();

//        _receiveShs->deleteLater();
        _receiveAis->deleteLater();
//        _receiveTtm->deleteLater();
//        _receiveNav->deleteLater();
//        _sendCmd->deleteLater();

        connectionStatus =false;

    }
}

void ShsClient::resetClient(){

    disconnectClient();
    connectClient();
}


void ShsClient::updateConnectionSettings()
{
    if(!connectionStatus)
    {
//        qShs = QString("Q_%1_SHS_SHIP%2_SYS%3").arg(appId).arg(shipId).arg(systemId);
//        qAis = QString("Q_%1_AIS_SHIP%2_SYS%3").arg(appId).arg(shipId).arg(systemId);
//        qNav = QString("Q_%1_GEN_SHIP%2_SYS%3").arg(appId).arg(shipId).arg(systemId);
//        qTtm = QString("Q_%1_TTM_SHIP%2_SYS%3").arg(appId).arg(shipId).arg(systemId);
//        qCmd = QString("Q_%1_CMD_SHIP%2_SYS%3").arg(appId).arg(shipId).arg(systemId);

//        foShs = QString("FO_ANS_SCC_%1").arg(shipId);
//        foAis = QString("FO_NMEA_AIS_%1").arg(shipId);
//        foNav = QString("FO_NMEA_GEN_%1").arg(shipId);
//        foTtm = QString("FO_NMEA_TTM_%1").arg(shipId);
//        foCmd = QString("FO_CONTROL_LINK_%1").arg(shipId);
//        qAis = QString("Q_SHIP_JSON_QT");
//        foAis =QString("EMSN_DATA_SIM");
//        qAis = "Q_AIS_WEB_FEEDER" ;
        qDebug()<<foAis;
//        foAis = "FO_NMEA_AIS_1";


//        _conStatusManager->connectionStatusWidget(0)->setServerComponents(server,qShs,foShs);
//        _conStatusManager->connectionStatusWidget(1)->setServerComponents(server,qAis,foAis);
//        _conStatusManager->connectionStatusWidget(2)->setServerComponents(server,qTtm,foTtm);
//        _conStatusManager->connectionStatusWidget(3)->setServerComponents(server,qCmd,foCmd);
    }
}


void ShsClient::createClients()
{
//    _receiveShs = new QAMQP::Receiver();
//    _receiveShs->setQueueName(qShs);
//    _receiveShs->setFanoutName(foShs);
//    _receiveShs->setServerAddress(amqpUrl);
//    _receiveShs->updateSettings();
//    connect(_receiveShs,SIGNAL(onMessageReceived(QByteArray)),_xmlParser,SLOT(parseXmlFromShs(QByteArray)));
//    connect(_receiveShs,SIGNAL(signalConnectionStatus(bool)),_conStatusManager->connectionStatusWidget(0),SLOT(setConnectionStatus(bool)));
//    connectShipDataToDataViewer(true);

    _receiveAis = new QAMQP::Receiver();
    _receiveAis->setQueueName(qAis);
    _receiveAis->setFanoutName(foAis);
    _receiveAis->setServerAddress(amqpUrl);
    _receiveAis->updateSettings();

//    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_readAis,SLOT(onDataReceived(QByteArray)),Qt::QueuedConnection);
    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),this,SIGNAL(signalAisMessageReceived(QByteArray)));
//    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->dataViewer(1),SLOT(setData(QByteArray)));
//    connect(_receiveAis,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->connectionStatusWidget(1),SLOT(onDataReceived()));
//    connect(_receiveAis,SIGNAL(signalConnectionStatus(bool)),_conStatusManager->connectionStatusWidget(1),SLOT(setConnectionStatus(bool)));
    connect(_receiveAis,SIGNAL(signalConnectionStatus(bool)),this, SIGNAL(signalReceiverConnectionStatus(bool)));

//    _receiveNav = new QAMQP::Receiver();
//    _receiveNav->setQueueName(qNav);
//    _receiveNav->setFanoutName(foNav);
//    _receiveNav->setServerAddress(amqpUrl);
//    _receiveNav->updateSettings();
//    connect(_receiveNav,SIGNAL(onMessageReceived(QByteArray)),_readGeneral,SLOT(onDataReceived(QByteArray)));
//    connect(_receiveNav,SIGNAL(onMessageReceived(QByteArray)),this,SIGNAL(signalGenMessageReceived(QByteArray)));


//    _receiveTtm = new QAMQP::Receiver();
//    _receiveTtm->setQueueName(qTtm);
//    _receiveTtm->setFanoutName(foTtm);
//    _receiveTtm->setServerAddress(amqpUrl);
//    _receiveTtm->updateSettings();
//    connect(_receiveTtm,SIGNAL(onMessageReceived(QByteArray)),_radar,SLOT(onPortReadTTM(QByteArray)));
//    connect(_receiveTtm,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->dataViewer(2),SLOT(setData(QByteArray)));
//    connect(_receiveTtm,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->connectionStatusWidget(2),SLOT(onDataReceived()));
//    connect(_receiveTtm,SIGNAL(signalConnectionStatus(bool)),_conStatusManager->connectionStatusWidget(2),SLOT(setConnectionStatus(bool)));


    //AMQP Sender for control nmea messages
//    _sendCmd = new QAMQP::Transmitter();
//    _sendCmd->setQueueName(qCmd);
//    _sendCmd->setQueueFlag(true);
//    _sendCmd->setFanoutName(foCmd);
//    _sendCmd->setFanoutFlag(true);
//    _sendCmd->setServerAddress(amqpUrl);
//    _sendCmd->updateSettings();
//    connect(_nmeaComposer,SIGNAL(sendControlMsg(QString)),_sendCmd,SLOT(sendMessage(QString)),Qt::QueuedConnection);
//    connect(_sendCmd,SIGNAL(signalConnectionStatus(bool)),_conStatusManager->connectionStatusWidget(3),SLOT(setConnectionStatus(bool)));
}

void ShsClient::purgeAllQueues()
{
    if(connectionStatus)
    {
//        _receiveShs->purgeQ();
        _receiveAis->purgeQ();
//        _receiveTtm->purgeQ();
//        _receiveNav->purgeQ();
    }
}


//XmlParser *ShsClient::xmlParser() const
//{
//    return _xmlParser;
//}

//void ShsClient::setXmlParser(XmlParser *xmlParser)
//{
//    _xmlParser = xmlParser;
//}

//NmeaComposer *ShsClient::nmeaComposer() const
//{
//    return _nmeaComposer;
//}

//void ShsClient::setNmeaComposer(NmeaComposer *nmeaComposer)
//{
//    _nmeaComposer = nmeaComposer;
//}

//ReadTtm *ShsClient::radar() const
//{
//    return _radar;
//}

//void ShsClient::setRadar(ReadTtm *radar)
//{
//    _radar = radar;
//}

//ReadAis *ShsClient::readAis() const
//{
//    return _readAis;
//}

//void ShsClient::setReadAis(ReadAis *readAis)
//{
//    _readAis = readAis;
//}

//ReadGeneral *ShsClient::readGeneral() const
//{
//    return _readGeneral;
//}

//void ShsClient::setReadGeneral(ReadGeneral *readGeneral)
//{
//    _readGeneral = readGeneral;
//}

void ShsClient::setAmqpServer(QString url, QString name)
{
    amqpUrl = url;
    server = name;
    resetClient();
}


//void ShsClient::connectShipDataToDataViewer(bool connected){
//    if(connected){
//            connect(_receiveShs,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->dataViewer(0),SLOT(setData(QByteArray)));
//            connect(_receiveShs,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->connectionStatusWidget(0),SLOT(onDataReceived()));

//    }else{
//            disconnect(_receiveShs,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->dataViewer(0),SLOT(setData(QByteArray)));
//            disconnect(_receiveShs,SIGNAL(onMessageReceived(QByteArray)),_conStatusManager->connectionStatusWidget(0),SLOT(onDataReceived()));

//    }
//}
