#include "connector.h"
#include "ui_connector.h"

Connector::Connector(QString AppId, int shipId, QWidget *parent) :
    QWidget(parent),
    shipId(shipId),
    appId(AppId),
    ui(new Ui::Connector)
{
    ui->setupUi(this);
    connectionStatus =false;
    shipIndex=0;
    userIndex=0;

    OnlineShip ship;
    ship.available = -1;
    for(int i=0;i<6;i++){
        availabilityMap[i+1] = ship;
    }

    ui->lbTitle->setText(QString("%1 Console %2").arg(AppId).arg(shipId));
}

Connector::~Connector()
{
    delete ui;
}


void Connector::on_pbSettings_clicked(bool checked)
{
    if(checked){
        ui->stackedWidget->setCurrentIndex(1);
        QTimer::singleShot(4000,this,SLOT(resetToShipSelector()));
    }
    else{
        resetToShipSelector();
    }
}

void Connector::resetToShipSelector(){
    ui->stackedWidget->setCurrentIndex(0);
    ui->pbSettings->setChecked(false);
}

void Connector::on_cbShip_activated(int index)
{
    shipIndex =index;

    if(availabilityMap.value(shipIndex+1).available){
        ui->pbConnect->setChecked(true);
        emit signalOnConnectionActivated(true,shipIndex,userIndex);
    }
    else{

        ui->pbConnect->setChecked(false);
        emit signalOnConnectionActivated(false,shipIndex,userIndex);
        ui->lbStatus->setText("Selected Ship is Offline!!");
    }

     sendStatusInfo();


 }

void Connector::on_cbSystem_activated(int index)
{

    userIndex =index;

    if(availabilityMap.value(shipIndex+1).available){
        ui->pbConnect->setChecked(true);
        emit signalOnConnectionActivated(true,shipIndex,userIndex);
    }
    else{
        ui->lbStatus->setText("Selected Ship is Offline!!");
    }
     sendStatusInfo();

}

void Connector::setConnectionIdentifiers(int shipIndex,int userIndex){
    ui->cbShip->setCurrentIndex(shipIndex);
    ui->cbSystem->setCurrentIndex(userIndex);

    this->shipIndex=shipIndex;
    this->userIndex =userIndex;
}

void Connector::connectToShip(int shsIndex,int userIndex,bool connect){
   if(connect)
   {
       setConnectionIdentifiers(shsIndex,userIndex);
   }
   on_pbConnect_clicked(connect);
   ui->pbConnect->setChecked(connect);
}

void Connector::on_pbConnect_clicked(bool checked)
{
    if(checked){
        if(availabilityMap.value(shipIndex+1).available)
        {
            emit signalOnConnectionActivated(true,shipIndex,userIndex);
        }
        else{
            ui->pbConnect->setChecked(false);
            ui->lbStatus->setText("Selected Ship is Offline!!");

        }

    }

    else{
        emit signalOnConnectionActivated(false,shipIndex,userIndex);
    }



}

void Connector::on_pbConnect_toggled(bool checked)
{
    connectionStatus = checked;


    if(checked){
        ui->pbConnect->setText("Disconnect");
    }
    else{
        ui->pbConnect->setText("Connect");
        ui->lbStatus->clear();
    }

    sendStatusInfo();
}


void Connector::osData(QMap<QString,QString> osData)
{
    if(connectionStatus){
        QString status = QString("Connected to Ship %1: %2").arg(shipIndex+1).arg(osData.value("name"));
        ui->lbStatus->setText(status);
    }
}

void Connector::setAvailable(int shsId, int availabilityCode, QString &server, QString &name)
{
    OnlineShip ship;
    ship.shipName =name;
    ship.server =server;
    ship.available =availabilityCode;




    if(!compareData(ship,availabilityMap.value(shsId)))
    {
        availabilityMap[shsId] = ship;
        populateShipList();
        sendStatusInfo();
    }
}

void Connector::populateShipList(){

    QMapIterator<int, OnlineShip> i(availabilityMap);
    QStringList itemList;
    while (i.hasNext()) {

        i.next();
        QString status;
        OnlineShip ship =i.value();

        QString name = ship.shipName;
        switch (ship.available){
        case 0:
            name ="";
            status ="Offline";
            break;

        case 1:
            status = "Online";
            break;

        case 2:
            name ="";
            status = "offline";
            break;
        default:
            break;
        }

        QString item = QString("Ship %1 %2").arg(i.key()).arg(name);
        itemList.append(item);
    }
    ui->cbShip->clear();
    ui->cbShip->addItems(itemList);
    ui->cbShip->setCurrentIndex(shipIndex);


}


bool Connector::compareData(OnlineShip first, OnlineShip second){

    if(first.available==second.available){
        return true;
    }
    else{

        return false;
    }
}

void Connector::sendStatusInfo(){

    OnlineShip selectedShip = availabilityMap.value(shipIndex+1);
    emit signalSelectedShipData(shipId,shipIndex,connectionStatus,selectedShip.available,selectedShip.server,selectedShip.shipName);

}



