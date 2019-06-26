#include "routearray.h"
#include "ui_routearray.h"
#include "iostream"

using namespace std;

RouteArray::RouteArray(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RouteArray)
{
    ui->setupUi(this);
    ui->RemoveWP->setDisabled(true);
    ui->routeTable->setColumnWidth(0,197);
    ui->routeTable->setColumnWidth(1,197);
    ui->routeTable->setColumnWidth(2,196);
    i = 1;

    _communicator =new Communicator("RA");
    connect(this, SIGNAL(serverSettings(QString,QString)), _communicator, SLOT(setAmqpServer(QString,QString)));
    connect(this, SIGNAL(sendDataToCommunicator(QByteArray)), _communicator,SIGNAL(sendTpMsg(QByteArray)));
    connect(this, SIGNAL(passTransmitterKey(QString)), _communicator,SLOT(setTransmitterKey(QString)));
}

RouteArray::~RouteArray()
{
    delete ui;
}

void RouteArray::userListUpdate(QMap<QString, UserInfo> list)
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

//    int size=0;
//    for(auto e : list.keys())
//    {
//        if(!userList.contains(e))
//        {
//            userList=list;
//            qDebug()<<"This is a new user";
//            QString value=userList.value(e).userName;
//            indexOfCombobox=ui->usersList_2->currentIndex();
//            ui->usersList_2->addItem(value);
//            ui->usersList_2->setCurrentIndex(indexOfCombobox);
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

//            indexOfCombobox=ui->usersList_2->currentIndex();
//            ui->usersList_2->clear();
//            for(auto e : list.keys())
//            {
//                ui->usersList_2->addItem(userList.value(e).userName);
//            }
//            ui->usersList_2->setCurrentIndex(indexOfCombobox);
//        }
//        enlisted=false;
//    }
}

void RouteArray::on_AddWP_clicked()
{
    ui->routeTable->insertRow(i);
    i++;
    if(i==10)
    {
        ui->AddWP->setDisabled(true);
    }
    if(i>=1)
    {
        ui->RemoveWP->setEnabled(true);
    }
//    qDebug()<<i;
}

void RouteArray::on_RemoveWP_clicked()
{
    i--;
    ui->routeTable->removeRow(i);

    if(i==1)
    {
        ui->RemoveWP->setDisabled(true);
    }
    if(i<=10)
    {
        ui->AddWP->setEnabled(true);
    }
//    qDebug()<<i;
}

void RouteArray::on_SendRouteMap_clicked()
{
    int routeId=ui->routeID->text().toInt();
    QString routeName;
    if(ui->routeName->text().isEmpty())
    {
        routeName="ship 1 Route";
    }
    else
        routeName=ui->routeName->text();

    QJsonArray route;

    for(int j=0;j<i;j++)
    {
        int wpId = j;
        double lat,lon,rad;
        QTableWidgetItem* item1 = ui->routeTable->item(j,0);
        QTableWidgetItem* item2 = ui->routeTable->item(j,1);
        QTableWidgetItem* item3 = ui->routeTable->item(j,2);
        if((!item1 ||item1->text().isEmpty()) || (!item2 ||item2->text().isEmpty()) || (!item3 ||item3->text().isEmpty()))
        {
            QMessageBox::information(this, "Warnng","Row "+QString::number(j+1)+" has empty item? Please fill these or remove the row");
            return;
        }

        lat = item1->text().toDouble();/*ui->routeTable->item(j,0)->text().toDouble();*/
        lon = item2->text().toDouble(); /*ui->routeTable->item(j,1)->text().toDouble();*/
        rad = item3->text().toDouble(); /*ui->routeTable->item(j,2)->text().toDouble();*/
//        qDebug()<<"Lat = "<<lat <<"Lon = "<<lon <<"Radius = " <<rad;

        QJsonObject jsonObj= QJsonObject{
        {"wpId", wpId},
        {"lat", lat},
        {"lon", lon},
        {"radius",rad},
    };
        route.push_back(jsonObj);
    }

    QJsonObject routeInfo = QJsonObject{
    {"routeId", routeId},
    {"routeName", routeName},
};

    QJsonObject data = QJsonObject{
    {"routeInfo", routeInfo},
    {"route", route},
};

    QJsonObject ShipData = QJsonObject{
    {"dataId", 2},
    {"data", data},
};
//    qDebug()<<route;
//    qDebug()<<data;
//    qDebug()<<ShipData;

    QJsonDocument ShipDoc = QJsonDocument(ShipData);
    emit (sendDataToCommunicator(ShipDoc.toJson()));
}

void RouteArray::on_usersList_activated(int index)
{
    QString transmiterKey;

    QString userId  = ui->usersList->currentData().toString();


    if(userList.contains(userId)){
        currentUserId =userId;
        currentUserName = userList.value(currentUserId).userName;
        ui->leCurrentUser->setText(currentUserName);
        transmiterKey =QString("K_%1").arg(userId);
        emit passTransmitterKey(transmiterKey);
//        qDebug() << userId  << " " << transmiterKey;
    }
}
