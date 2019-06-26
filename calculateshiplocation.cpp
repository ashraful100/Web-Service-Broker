#include "calculateshiplocation.h"
#include "ui_calculateshiplocation.h"

CalculateShipLocation::CalculateShipLocation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalculateShipLocation)
{
    ui->setupUi(this);
    ui->displayLocation->setReadOnly(true);
    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(dataUpdate()));

    //connect(ui->dialWidget, SIGNAL(sendAngle(double)), this, SLOT(displayAngle(double)));
    indexOfCombobox=-1;
    ui->usersList->setCurrentIndex(indexOfCombobox);
}

void CalculateShipLocation::displayJson(QByteArray array)
{
    QJsonDocument docs= QJsonDocument::fromJson(array);

    qDebug()<<"Received from "<<publisher;
    qDebug()<<"Latitude & Longitude "<< docs.object().value("latlon");
    qDebug()<<"Rotation "<< docs.object().value("Rotation");
    qDebug()<<"Speed "<< docs.object().value("Speed");
    qDebug()<<"Duration "<< docs.object().value("Interval");
}

void CalculateShipLocation::dataUpdate()
{
    QMap<QString,QString> decodedData;
    decodedData["lat"] = QString::number(Lat,'f',4);
    decodedData["lon"] = QString::number(Lon,'f',4);
    decodedData["name"] = ui->leName->text();
    decodedData["cog"] = ui->angle->text();
    decodedData["hdg"] = ui->angle->text();
    decodedData["sog"] = ui->speed->text();

    dataUpdate(decodedData);


    //    QJsonArray route;
    //    QJsonObject jsonObj= QJsonObject{
    //    {"wpId", id},
    //    {"lat", Lat},
    //    {"lon", Lon},
    //    {"radius", "1"},
    //};
    //    route.push_back(jsonObj);

    //    QJsonObject routeInfo = QJsonObject{
    //    {"routeId", routeId},
    //    {"routeName", Name},
    //};

    //    QJsonObject data = QJsonObject{
    //    {"routeInfo", routeInfo},
    //    {"route", route},
    //};

    //    QJsonObject ShipData = QJsonObject{
    //    {"dataId", 2},
    //    {"data", data},
    //};
    //    QJsonDocument shipDoc = QJsonDocument(ShipData);

    //    emit sendDataToCommunicator(shipDoc.toJson());


    //    QString data2(shipDoc.toJson(QJsonDocument::Compact));
    //    qDebug()<<ShipData;

    //    data2.remove(QRegularExpression("[{}]"));
    //    data2.remove('"');
    //    data2.remove('[');
    //    data2.remove(']');
    //    data2.replace(",","\n");

    //    ui->displayLocation->setAlignment(Qt::AlignLeft);
    //    ui->displayLocation->append(QString("%2 %1").arg(QDateTime::currentDateTime().toString("dd MM yyyy hh:mm:ss"))
    //                                .arg(data2));

    QGeoCoordinate initPos(Lat,Lon);

    initPos =initPos.atDistanceAndAzimuth(Speed*Time,Angle);

    Lat=initPos.latitude();
    Lon=initPos.longitude();
    routeId++;


}

void CalculateShipLocation::dataUpdate(QMap<QString,QString> decodedData)
{
    //Parse QMap with AIS decoded data
    int mmsi = decodedData.value("mmsi").toInt();
    double lat = decodedData.value("lat").toDouble();
    double lon = decodedData.value("lon").toDouble();

    QString name = decodedData.value("name");
    QString destination = decodedData.value("destination");
    double cog = decodedData.value("cog").toDouble();
    double hdg = decodedData.value("hdg").toDouble();
    QString callsign = decodedData.value("callsign");
    QString msg = decodedData.value("message");
    QString msgName=decodedData.value("messageName");

    int timestamp=decodedData.value("timestamp").toInt();
    int to_bow= decodedData.value("to_bow").toInt();
    int to_port=decodedData.value("to_port").toInt();
    //    int to_starboard=decodedData.value("to_starboard").toInt();
    int to_stern=decodedData.value("to_stern", "125").toInt();

    // Create and populate ship JSON Object
    QJsonObject shipObj = QJsonObject {
    {"id", mmsi},
    {"name" , name},
    {"destination", destination},
    {"lat" , lat},
    {"lon" , lon},
    {"cog" , cog},
    {"hdg", hdg},
    {"rotation", hdg},
    {"callsign", callsign},
    {"message", msg},
    {"messageName",msgName},
    {"timestamp",timestamp},
    {"to_bow",to_bow},
    {"to_port",to_port},
    {"to_starboard",to_port},
    {"to_stern",to_stern}
};
    // Create and populate Data JSON Object (Data template refer #24 in GIT Lab)
    QJsonObject dataObj = QJsonObject{
    {"dataId", 1},
    {"data",shipObj}
};
    // Create JSOn Document
    QJsonDocument shipDataDoc=QJsonDocument{dataObj};
    qDebug()<< "shipDoc"<<shipDataDoc;
    //    publish.sendJsonDoc(shipDoc, ui->receiver->text());/*"KEY_NODE")*/
    // Send Document

    emit sendDataToCommunicator(shipDataDoc.toJson());
}

CalculateShipLocation::~CalculateShipLocation()
{
    timer->stop();
    delete ui;
}

void CalculateShipLocation::on_calculate_2_clicked()
{
    double lat=ui->latitude->text().toDouble();
    double lon=ui->longitude->text().toDouble();
    double speed=ui->speed->text().toDouble();
    double angle=ui->angle->text().toDouble();
    float time=ui->setTime->text().toFloat();

    QGeoCoordinate initPos (lat,lon);

    initPos =initPos.atDistanceAndAzimuth(speed*time,angle);

    ui->latitude->setText(QString::number(initPos.latitude()));
    ui->longitude->setText(QString::number(initPos.longitude()));

    qDebug()<<initPos;
}

void CalculateShipLocation::on_sendPosition_clicked()
{       
    if(ui->receiver->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Error"),tr("Please give Receiver name."));
    }
    else
    {
        Name=ui->leName->text();
        //        Destination=ui->lbDestination->text();
        Lat=ui->latitude->text().toDouble();
        Lon=ui->longitude->text().toDouble();
        Angle=ui->angle->text().toDouble();
        Speed=ui->speed->text().toDouble();
        Time=ui->setTime->text().toDouble();


        timer->start(1000);
    }
}

void CalculateShipLocation::on_stopSending_clicked()
{
    ui->latitude->setText(QString::number(Lat));
    ui->longitude->setText(QString::number(Lon));
    disconnect(timer, SIGNAL(timeout()), this, SLOT(dataUpdate()));
}

void CalculateShipLocation::setId(int value)
{
    id = value;
    ui->lbShipId->setText(QString::number(value));
}

void CalculateShipLocation::userListUpdate(QMap<QString, UserInfo> list)
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
    //    qDebug() <<
    //    int size=0;
    //    for(auto e : list.keys())
    //    {
    //        if(!userList.contains(e))
    //        {
    //            userList=list;
    //            qDebug()<<"This is a new user";
    //            QString userName=userList.value(e).userName;
    //            QString userId=userList.value(e).userId;
    //            indexOfCombobox=ui->usersList->currentIndex();
    //            ui->usersList->addItem(userName,QVariant(userId));
    //            ui->usersList->setCurrentIndex(indexOfCombobox);
    //            enlisted= false;
    //            size++;
    //        }
    //    }
    //    if(size==0)
    //    {
    //        enlisted=true;
    //    }
    //    if(enlisted)
    //    {
    //        for(auto e : list.keys())
    //        {
    //            qDebug()<<"This is a old enlisted user";
    //            userList=list;

    //            indexOfCombobox=ui->usersList->currentIndex();
    //            ui->usersList->clear();
    //            for(auto e : list.keys())
    //            {
    //                ui->usersList->addItem(userList.value(e).userName);
    //            }
    //            ui->usersList->setCurrentIndex(indexOfCombobox);
    //        }
    //        enlisted=false;
    //    }
}

//void CalculateShipLocation::on_usersList_currentIndexChanged(const QString &arg1)
//{

//    if(!enlisted && (indexOfCombobox !=ui->usersList->currentIndex()))
//    {
//        for(auto e : userList.keys())
//        {
//            if(userList.value(e).userName==ui->usersList->currentText())
//            {
//                qDebug()<<"This is"<<ui->usersList->currentText()<<"with UserId"<<e;
//                transmitterKey="K_"+e;
//                qDebug()<<transmitterKey;
//                emit(sendTransmitterKey(transmitterKey));
//            }
//        }
//    }
//}

void CalculateShipLocation::displayAngle(double angle)
{
    ui->angle->setText(QString::number(angle));
}

void CalculateShipLocation::on_usersList_activated(int index)
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
