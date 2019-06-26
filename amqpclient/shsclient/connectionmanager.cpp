#include "connectionmanager.h"
#include "ui_connectionmanager.h"

ConnectionManager::ConnectionManager(QString AppId, int shipId, int systemId, CallBack *_callBack, QWidget *parent) :
    QWidget(parent),appId(AppId),id(shipId),
    ui(new Ui::ConnectionManager)
{
    ui->setupUi(this);
    qRegisterMetaType< QMap<QString,QString> >("QMap<QString,QString>");
    //For setting the icon in the ship information widget
    //The connection manager will show the app icon as the icon in ship info
    QString iconPath,hoverIconPath,ansToSoc,socToAns;
    if(AppId.contains("ANS"))
    {
        iconPath = ":/appicons/ansapp.png";
        hoverIconPath = ":/appicons/ansapp_hover.png";
        ui->pbIn->setText("SOC to ANS");
        ui->pbOut->setText("ANS to SOC");
    }

    else {
        iconPath = ":/appicons/socapp.png";
        hoverIconPath = ":/appicons/socapp_hover.png";
        ui->pbOut->setText("SOC to ANS");
        ui->pbIn->setText("ANS to SOC");
    }
    _shipInfoWidget =new ShipInfoWidget();
    _shipInfoWidget->setIcon(iconPath,hoverIconPath);
    _shipInfoWidgetLite =new ShipInfoWidgetLite();
    //end ship info incon

    ui->swCommunicationStatus->addWidget(new ConnectionStatusManager());
    ui->swCommunicationStatus->addWidget(new ConnectionStatusManager());
    ui->swCommunicationStatus->addWidget(new ConnectionStatusManager());

    buttonGroup.addButton(ui->pbShs,0);
    buttonGroup.addButton(ui->pbIn,1);
    buttonGroup.addButton(ui->pbOut,2);

    connect(&buttonGroup,SIGNAL(buttonClicked(int)),ui->swCommunicationStatus,SLOT(setCurrentIndex(int)));
    buttonGroup.setExclusive(true);
    buttonGroup.button(0)->setChecked(true);
    ui->swCommunicationStatus->setCurrentIndex(0);

    _shsClient = new ShsClient(AppId,(ConnectionStatusManager*)ui->swCommunicationStatus->widget(0));

    _communicator =new Communicator(AppId,(ConnectionStatusManager*)ui->swCommunicationStatus->widget(1),(ConnectionStatusManager*)ui->swCommunicationStatus->widget(2),this);
    _communicator->setConnectionIdentifiers(shipId,systemId);


    _shsClient->setConnectionIdentifiers(shipId,systemId);

    _connector =new Connector(AppId,shipId);
    _connector->setConnectionIdentifiers(shipId-1,systemId-1); //ID to index
    connect(_connector,SIGNAL(signalOnConnectionActivated(bool,int,int)),this,SLOT(slotOnConnectorActivated(bool,int,int)));
    connect(_connector,SIGNAL(signalOnConnectionActivated(bool,int,int)),this,SIGNAL(signalOnConnectorActivated(bool,int,int)));

    connect(_shsClient,SIGNAL(signalAisMessageReceived(QByteArray)),this,SIGNAL(signalAisMessageReceived(QByteArray)));
    connect(_shsClient,SIGNAL(signalGenMessageReceived(QByteArray)),this,SIGNAL(signalGenMessageReceived(QByteArray)));
    connect(_shsClient,SIGNAL(signalTtmMessageReceived(QByteArray)),this,SIGNAL(signalTtmMessageReceived(QByteArray)));
    connect(_shsClient,SIGNAL(signalShsMessageReceived(QByteArray)),this,SIGNAL(signalShsMessageReceived(QByteArray)));

    _shsData =new ShsData();


    connect(_shsClient->xmlParser(),SIGNAL(shsDataReceived(QMap<QString,QString>)),_shsData,SLOT(onShsRead(QMap<QString,QString>)));
    connect(_shsData,SIGNAL(osObject(QMap<QString,QString>)),this,SIGNAL(signalOsData(QMap<QString,QString>)));
    connect(_shsData,SIGNAL(osObject(QMap<QString,QString>)),_shipInfoWidget,SLOT(setOsData(QMap<QString,QString>)));
    connect(_shsData,SIGNAL(osObject(QMap<QString,QString>)),_shipInfoWidgetLite,SLOT(setOsData(QMap<QString,QString>)));
    connect(_shsData,SIGNAL(osObject(QMap<QString,QString>)),_connector,SLOT(osData(QMap<QString,QString>)));
    connect(_shsData,SIGNAL(osObject(QMap<QString,QString>)),_shsClient->radar(),SLOT(setOsData(QMap<QString,QString>)));


    _trafficShipData =new TrafficShipData();
    connect(this,SIGNAL(signalOnAisRangeChanged(double)),_trafficShipData,SLOT(setAisRange(double)));
    connect(_shsData,SIGNAL(osObject(QMap<QString,QString>)),_trafficShipData,SLOT(setOsData(QMap<QString,QString>)));
    connect(_shsClient->radar(),SIGNAL(sendTtmTargetList(QList<QStringList>)),_trafficShipData,SLOT(setTtmData(QList<QStringList>)));
    connect(_trafficShipData,SIGNAL(signalOwnShipAisData(QMap<QString,QString>)),_shsClient->readGeneral(),SLOT(slotSetOsAisdata(QMap<QString,QString>)));
    _aisDecoder =new AisDecoder(shipId,_callBack);




    switch (shipId) {
    case 1:
        connect(_callBack,SIGNAL(signalAisDecodedData1(EcAISTargetInfo*)),_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)));
        break;
    case 2:
        connect(_callBack,SIGNAL(signalAisDecodedData2(EcAISTargetInfo*)),_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)));
        break;
    case 3:
        connect(_callBack,SIGNAL(signalAisDecodedData3(EcAISTargetInfo*)),_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)));
        break;
    case 4:
        connect(_callBack,SIGNAL(signalAisDecodedData4(EcAISTargetInfo*)),_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)));
        break;
    case 5:
        connect(_callBack,SIGNAL(signalAisDecodedData5(EcAISTargetInfo*)),_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)));
        break;
    case 6:
        connect(_callBack,SIGNAL(signalAisDecodedData6(EcAISTargetInfo*)),_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)));
        break;
    default:
        break;
    }

    //connect(_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)),_aisDecoder,SLOT(print1(EcAISTargetInfo*)));


    connect(_shsClient,SIGNAL(signalAisMessageReceived(QByteArray)),_aisDecoder,SLOT(onAisDataReceivedFromShs(QByteArray)));
    connect(_aisDecoder,SIGNAL(signalAisDecodedData(EcAISTargetInfo*)),_trafficShipData,SIGNAL(parseAisData(EcAISTargetInfo*)));


    //Traffic Ship Selector
    _tsDataSwitch =new TsDataSwitch();
    connect(this,SIGNAL(signalOnTsDataSourceChanged(int)),_tsDataSwitch,SLOT(OnTsDataSourceChanged(int)));
    connect(this, SIGNAL(signalOnTsDataSourceChanged(int)), _trafficShipData, SLOT(onTsDataSourceChanged(int)));
    connect(_shsData,SIGNAL(tsObject(QMap<QString,QString>)),_tsDataSwitch,SLOT(onTsShsDataReceived(QMap<QString,QString>)));
    connect(_trafficShipData,SIGNAL(signalTrafficShipAisData(QMap<QString,QString>)),_tsDataSwitch,SLOT(onTsAisDataReceived(QMap<QString,QString>)));
    connect(_tsDataSwitch,SIGNAL(signalTsData(QMap<QString,QString>)),this,SIGNAL(signalTsData(QMap<QString,QString>)));

    //Ais Data Switch
    connect(this,SIGNAL(signalOnAisDataSourceChanged(int)),_aisDecoder,SLOT(setAisSource(int)));
    //Only test

    //connect(&comTester,SIGNAL(sendTpMsg(QByteArray)),_communicator,SIGNAL(sendTpMsg(QByteArray)));
    //connect(&comTester,SIGNAL(sendWrMsg(QByteArray)),_communicator,SIGNAL(sendWrMsg(QByteArray)));
    //connect(&comTester,SIGNAL(sendCaMsg(QByteArray)),_communicator,SIGNAL(sendCaMsg(QByteArray)));
    //connect(&comTester,SIGNAL(sendVoMsg(QByteArray)),_communicator,SIGNAL(sendVoMsg(QByteArray)));

    //connect(_communicator,SIGNAL(onTpMsgReceived(QByteArray)),&comTester,SLOT(onTpMsgReceived(QByteArray)));
    //connect(_communicator,SIGNAL(onWrMsgReceived(QByteArray)),&comTester,SLOT(onWrMsgReceived(QByteArray)));
    //connect(_communicator,SIGNAL(onCaMsgReceived(QByteArray)),&comTester,SLOT(onCaMsgReceived(QByteArray)));
    //connect(_communicator,SIGNAL(onVoMsgReceived(QByteArray)),&comTester,SLOT(onVoMsgReceived(QByteArray)));
    //Move Shs Data to Thread

//    _shsData->moveToThread(&shsDataThread);
//    shsDataThread.start();

    slotOnOsDataSourceChanged(3);
}

ConnectionManager::~ConnectionManager()
{


    delete ui;
//    delete _tsDataSwitch;
//    delete _aisDecoder;
//    delete _trafficShipData;
//    delete _shsData;
//    delete _shsClient;
//    delete _communicator;
//    delete _shipInfoWidget;
//    delete _shipInfoWidgetLite;
//    shsDataThread.quit();
//    shsDataThread.wait();
}


ShsClient* ConnectionManager::getShsClient()
{
    return _shsClient;
}

void ConnectionManager::connectClient(bool checked)
{
    if(checked)
    {
        _shsClient->connectClient();
        _communicator->connectClient();

    }
    else
    {
        _shsClient->disconnectClient();
        _communicator->disconnectClient();

    }
}

void ConnectionManager::slotOnConnectorActivated(bool connected,int shipIndex,int userIndex)
{
    _shsClient->setConnectionIdentifiers(shipIndex+1,userIndex+1);
    _communicator->setConnectionIdentifiers(shipIndex+1,userIndex+1);

    //reset
    connectClient(connected);

}
ShipInfoWidget *ConnectionManager::shipInfoWidget() const
{
    return _shipInfoWidget;
}

ShipInfoWidgetLite *ConnectionManager::shipInfoWidgetLite() const
{
    return _shipInfoWidgetLite;
}


Connector *ConnectionManager::connector() const
{
    return _connector;
}

TrafficShipData *ConnectionManager::trafficShipData() const
{
    return _trafficShipData;
}

ShsClient *ConnectionManager::shsClient() const
{
    return _shsClient;
}

Communicator *ConnectionManager::communicator() const
{
    return _communicator;
}

//--------------------------**************----------------------//
// Remote control messages

void ConnectionManager::parseRemoteControlMessage(QByteArray remoteMsgAry){

    QString remoteMsg = QString(remoteMsgAry);
    QStringList remoteMsgList = remoteMsg.split(";");

    bool convSuccessInt =false;
    int remoteMsgSourceId = remoteMsgList.value(0).toInt(&convSuccessInt);
    if(!convSuccessInt)
        return;

    switch (remoteMsgSourceId) {
    case RemoteId::VIRTUALSHS:

        break;
    case RemoteId::ANS:
        if(appId.contains("ANS"))
        {
            processRemoteControlMsg(remoteMsgList);
        }
        break;

    case RemoteId::SOC:
        if(appId.contains("SOC"))
        {
            processRemoteControlMsg(remoteMsgList);
        }
        break;
    default:
        break;
    }
}

void ConnectionManager::slotOnOsDataSourceChanged(int sourceId)
{
    QString  server("Local");
    QString name("AIS Source");
    switch (sourceId) {
    case 1:
        disconnect(_trafficShipData,SIGNAL(signalOwnShipAisData(QMap<QString,QString>)),_shsData,SIGNAL(osObject(QMap<QString,QString>)));
        break;
    case 2:
        disconnect(_trafficShipData,SIGNAL(signalOwnShipAisData(QMap<QString,QString>)),_shsData,SIGNAL(osObject(QMap<QString,QString>)));
        break;
    case 3:
        connect(_trafficShipData,SIGNAL(signalOwnShipAisData(QMap<QString,QString>)),_shsData,SIGNAL(osObject(QMap<QString,QString>)));
        emit signalDataAvailable(1,1,server,name);
        break;
    default:
        break;
    }
}


void ConnectionManager::processRemoteControlMsg(QStringList &remoteMsgList){

    bool convSuccessInt =false;

    int controller = remoteMsgList.value(1).toInt(&convSuccessInt);
    if(!convSuccessInt)
        return;

    switch (controller) {
    case RemoteId::Control:
        parseTypeControllerMessage(remoteMsgList);
        break;
    case RemoteId::Scenario:

        break;

    default:
        break;
    }

}

void ConnectionManager::parseTypeControllerMessage(QStringList &controlMsg){

    bool convSuccessInt =false;
    int control = controlMsg.value(3).toInt(&convSuccessInt);
    if(!convSuccessInt)
        return;

    convSuccessInt =false;
    int shs =controlMsg.value(2).toInt(&convSuccessInt);
    if(!convSuccessInt)
        return;
    if(shs!=id)
        return;

    convSuccessInt =false;
    int value1 =controlMsg.value(4).toInt(&convSuccessInt);
    if(!convSuccessInt)
        return;

    convSuccessInt =false;
    int value2 =controlMsg.value(5).toInt(&convSuccessInt);
    int value3 =controlMsg.value(6).toInt(&convSuccessInt);

    //    if(!convSuccessInt)
    //        return;

    switch (control) {
    case RemoteId::Connect:
        _connector->connectToShip(value2,value3,value1);
        break;
    case RemoteId::Disconnect:
        _connector->connectToShip(value2,value3,value1);
        break;
    case RemoteId::Reset:

        break;

    default:
        break;
    }


}
