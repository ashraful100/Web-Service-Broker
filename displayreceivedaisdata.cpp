#include "displayreceivedaisdata.h"
#include "ui_displayreceivedaisdata.h"

void DisplayReceivedAISData::userListUpdate(QMap<QString, UserInfo> list)
{
    ui->usersList->clear();
    userList=list;

    QMapIterator<QString, UserInfo> userIterator(list);
    while (userIterator.hasNext()) {
        userIterator.next();

        QString userName=userIterator.value().userName;
        QString userId=userIterator.key();
        ui->usersList->addItem(userName,userId);
    }
    if(!userList.contains(currentUserId)){
        //TODO Disconnect the current connect, Stop sending and notify the user
    }
    // Set the current index in the combo box to the currently connected user
    int indexOfConnectedUser = ui->usersList->findData(QVariant(currentUserId));
    ui->usersList->setCurrentIndex(indexOfConnectedUser);
}

void DisplayReceivedAISData::receiveID(int id)
{
    QString idName = "FO_NMEA_AIS_"+QString::number(id);
    QString qName= "Q_AIS_SHIP_"+QString::number(id);
    _shsclient =new ShsClient(idName,qName); /*"FO_NMEA_AIS_1"*/
    connect(this, SIGNAL(setServerAddress(QString,QString)),_shsclient, SLOT(setAmqpServer(QString,QString)));
    connect(_shsclient, SIGNAL(signalAisMessageReceived(QByteArray)),this, SIGNAL(aisData(QByteArray)));
    connect(_shsclient, SIGNAL(signalReceiverConnectionStatus(bool)), this, SIGNAL(sendConnectionStatus(bool)));
}

void DisplayReceivedAISData::displayReceivedAis(QByteArray ais)
{
    _aisDecoder->onAISMessageReceived(ais);
    qDebug()<<"Displaying AIS"<<ais;
    ui->receivedAis->append(ais);
}


DisplayReceivedAISData::DisplayReceivedAISData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayReceivedAISData)
{
    ui->setupUi(this);
    _aisDecoder = new AISDecoderLib;
    connect(_aisDecoder, SIGNAL(signalDecodedAisMessage(QMap<QString,QString>)),this, SLOT(slotOnAisDecodedReceived(QMap<QString,QString>)));
}

void DisplayReceivedAISData::setShipID(int i)
{
    ui->iD->setText(QString::number(i));
    receiveID(i);
}

DisplayReceivedAISData::~DisplayReceivedAISData()
{
    _aisDecoder->deleteLater();
    _shsclient->deleteLater();
    delete ui;
}

void DisplayReceivedAISData::composeJSONfromDecodedAIS(int mmsi)
{
    if(!shipDataMapStatic.contains(mmsi)){
        return;
    }
    QMap<QString,QString> staticDataMap =shipDataMapStatic[mmsi];
    QMap<QString,QString> dynamicDataMap =shipDataMapDynamic[mmsi];

    double lat = dynamicDataMap.value("lat").toDouble();
    double lon = dynamicDataMap.value("lon").toDouble();

    double cog = dynamicDataMap.value("cog").toDouble();
    double hdg = dynamicDataMap.value("hdg").toDouble();


    QString name = staticDataMap.value("name");

    QString destination = staticDataMap.value("destination");
    QString callsign = staticDataMap.value("callsign");

    int timestamp=dynamicDataMap.value("timestamp").toInt();
    int to_bow= staticDataMap.value("to_bow").toInt();
    int to_port=staticDataMap.value("to_port").toInt();
    int to_starboard=staticDataMap.value("to_starboard").toInt();
    int to_stern=staticDataMap.value("to_stern", "125").toInt();

    int loa = staticDataMap.value("to_bow").toInt()+ staticDataMap.value("to_stern").toInt();
    int boa = staticDataMap.value("to_port").toInt()+ staticDataMap.value("to_starboard").toInt();

    // Create and populate ship JSON Object
    QJsonObject shipObj
    {
        {"id", mmsi},
        {"name" , name},
        {"destination", destination},
        {"lat" , lat},
        {"lon" , lon},
        {"cog" , cog},
        {"hdg", hdg},
        {"rotation", hdg},
        {"callsign", callsign},
        {"loa", loa},
        {"boa",boa},
        {"timestamp",timestamp},
        {"to_bow",to_bow},
        {"to_port",to_port},
        {"to_starboard",to_starboard},
        {"to_stern",to_stern}
    };

    // Create and populate Data JSON Object (Data template refer #24 in GIT Lab)
    QJsonObject dataObj{
        {"dataId", 1},
        {"data",shipObj}
    };

    // Create JSON Document
    shipDataDoc=QJsonDocument(dataObj);
    //    qDebug()<< "shipDoc"<<shipDataDoc;
    //    publish.sendJsonDoc(shipDoc, ui->receiver->text());/*"KEY_NODE")*/


    emit emitAisData(shipDataDoc.toJson());
}

void DisplayReceivedAISData::slotOnAisDecodedReceived(QMap<QString, QString> decodedAis)
{
    int messageId =decodedAis["message"].toInt();
    int mmsi =decodedAis["mmsi"].toInt();

    switch (messageId) {
    case 1:  // Dynamic Message
        updateDynamicData(decodedAis);
        break;
    case 5:  // Static
        updateStaticData(decodedAis);
        break;
    default:
        break;
    }
    composeJSONfromDecodedAIS(mmsi);
    displayDataInUI();
}

void DisplayReceivedAISData::on_pushButton_clicked()
{
    connect(this, SIGNAL(aisData(QByteArray)), this, SLOT(displayReceivedAis(QByteArray)));
}

void DisplayReceivedAISData::on_pushButton_2_clicked()
{
    disconnect(this, SIGNAL(aisData(QByteArray)), this, SLOT(displayReceivedAis(QByteArray)));
}

void DisplayReceivedAISData::on_sendAis_clicked()
{
    connect(this,SIGNAL(emitAisData(QByteArray)),this, SLOT(sendAisData(QByteArray)));
}

void DisplayReceivedAISData::on_stopSendingAis_clicked()
{
    disconnect(this,SIGNAL(emitAisData(QByteArray)),this, SLOT(sendAisData(QByteArray)));
}

void DisplayReceivedAISData::sendAisData(QByteArray array)
{
    qDebug()<<"Sending AIS";
    //qDebug()<<QJsonDocument::fromJson(array) ;
    emit sendDataToCommunicator(array);
}


//void DisplayReceivedAISData::on_cbShip_activated(int index)
//{
//    int displayMMSI = ui->cbShip->currentData().toInt();

//    if(shipDataMapStatic.contains(displayMMSI)){
//        this->displayMMSI =displayMMSI;
//    }
//}

void DisplayReceivedAISData::on_pbRefresh_clicked()
{
    shipDataMapStatic.clear();
    shipDataMapDynamic.clear();
    ui->cbShip->clear();

}



void DisplayReceivedAISData::displayDataInUI(){

    if(!shipDataMapStatic.contains(displayMMSI)){
        return;
    }

    QMap<QString,QString> staticDataMap =shipDataMapStatic[displayMMSI];
    QMap<QString,QString> dynamicDataMap =shipDataMapDynamic[displayMMSI];
    ui->lbShipName->setText(staticDataMap["name"]);
    ui->lbMmsi->setText(dynamicDataMap["mmsi"]);
    ui->lbLat->setText(dynamicDataMap["lat"]);
    ui->lbLon->setText(dynamicDataMap["lon"]);

    ui->lbHdg->setText(dynamicDataMap["hdg"]);
    ui->lbCog->setText(dynamicDataMap["cog"]);

    QString eta = QString("%1/%2 %3:%4")
            .arg(staticDataMap.value("eta_day"))
            .arg(staticDataMap.value("eta_month"))
            .arg(staticDataMap.value("eta_hour"))
            .arg(staticDataMap.value("eta_min"));

    ui->lbEtaAis->setText(eta);

    ui->lbCallsign->setText(staticDataMap.value("callsign"));
    ui->lbImo->setText(staticDataMap.value("imo"));
    ui->lbToPort->setText(staticDataMap.value("to_port"));
    ui->lbToSb->setText(staticDataMap.value("to_starboard"));
    ui->lbToStern->setText(staticDataMap.value("to_stern"));
    ui->lbToBow->setText(staticDataMap.value("to_bow"));

    int loa = staticDataMap.value("to_bow").toInt()+ staticDataMap.value("to_stern").toInt();
    int boa = staticDataMap.value("to_port").toInt()+ staticDataMap.value("to_starboard").toInt();

    ui->lbBoa->setText(QString::number(boa));
    ui->lbLoa->setText(QString::number(loa));
    ui->lbDraught->setText(staticDataMap.value("draught"));
    ui->lbDestination->setText(staticDataMap.value("destination"));

    ui->lbNavStatus->setText(dynamicDataMap.value("navStatusName"));
    bool positionAccuracy =dynamicDataMap.value("positionAccuracy").toInt();
    if(positionAccuracy){
        ui->lbPosAccuracy->setText("High (< 10 m)");
    } else{
        ui->lbPosAccuracy->setText("Low (> 10 m)");
    }
    bool raim =dynamicDataMap.value("radioStatus").toInt();

    if(raim){
        ui->lbRaim->setText("RAIM In Use");
    } else{
        ui->lbRaim->setText("Not in use (default)");
    }
    ui->lbManInd->setText(dynamicDataMap.value("man_indicatorName"));

    QString aisTimeStampStr ="";
    int aisTimeStamp =dynamicDataMap.value("timestamp").toInt();
    if(aisTimeStamp<60){
        aisTimeStampStr =dynamicDataMap.value("timestamp");
    } else{
        aisTimeStampStr = "invalid";
    }

}

void DisplayReceivedAISData::updateStaticData(QMap<QString, QString> data)
{
    int mmsi = data.value("mmsi").toInt();
    QString name = data.value("name");
    if(name.isEmpty()){
        return;
    }
    if(!shipDataMapStatic.contains(mmsi)&&!name.isEmpty()){

        QString shipInList =QString("%1 %2").arg(mmsi).arg(name);
        ui->cbShip->addItem(shipInList,QVariant(mmsi));
    }
    shipDataMapStatic[mmsi] =data;
}

void DisplayReceivedAISData::updateDynamicData(QMap<QString, QString> data)
{
    int mmsi = data.value("mmsi").toInt();

    if(shipDataMapStatic.contains(mmsi)){

        shipDataMapDynamic[mmsi] = data;
    }
}

void DisplayReceivedAISData::on_usersList_activated(int index)
{
    QString transmiterKey;

    QString userId  = ui->usersList->currentData().toString();

    if(userList.contains(userId)){
        currentUserId =userId;
        currentUserName = userList.value(currentUserId).userName;
        ui->leCurrentUser->setText(currentUserName);
        transmiterKey =QString("K_%1").arg(userId);
        emit sendTransmitterKey(transmiterKey);
        qDebug() << userId  << " " << transmiterKey;
    }
}

void DisplayReceivedAISData::on_receivedAis_textChanged()
{
    if(ui->receivedAis->toPlainText().length()>10000)
    {
        ui->receivedAis->clear();

    }
}
