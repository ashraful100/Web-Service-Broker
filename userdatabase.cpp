#include "userdatabase.h"
#include "ui_userdatabase.h"
#include "QDateTime"

UserDatabase::UserDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDatabase)
{
    ui->setupUi(this);
    setServer=true;
    _communicatorRcvOwnShip = new Communicator("DBRCVO");
    _communicatorRcvTrafficShip= new Communicator("DBRCVT");
    _communicatorSend = new Communicator("DBSEND");
    dbWriter = new DbWriter(this);   

    connect(ui->dbServer, SIGNAL(signalOnServerChanged(int,QString,QString)),this, SLOT(receiveServerUrl(int,QString,QString)));

    connect(ui->rcvDatabaseServer, SIGNAL(signalOnServerChanged(QString,QString)), _communicatorRcvOwnShip, SLOT(setAmqpServer(QString,QString)));
    connect(ui->rcvDatabaseServer_2, SIGNAL(signalOnServerChanged(QString,QString)), _communicatorRcvTrafficShip, SLOT(setAmqpServer(QString,QString)));

    connect(_communicatorRcvOwnShip,SIGNAL(onTpMsgReceived(QByteArray)),this,SLOT(displayMsg(QByteArray)));
    connect(_communicatorRcvTrafficShip,SIGNAL(onTpMsgReceived(QByteArray)),this,SLOT(displayMsg(QByteArray)));
    connect(dbWriter, SIGNAL(dummyOwnShipJsonForTesting(QByteArray)),this, SLOT(displayMsg(QByteArray)));
    connect(dbWriter, SIGNAL(dummyTrafficShipJsonForTesting(QByteArray)),this, SLOT(displayMsg(QByteArray)));

    connect(ui->sendDatabaseServer, SIGNAL(signalOnServerChanged(QString,QString)),_communicatorSend, SLOT(setAmqpServer(QString,QString)));

    connect(_communicatorRcvOwnShip, SIGNAL(signalReceiverConnectionStatus(bool)), ui->wRcvSignal, SLOT(setTraficSignal(bool)));
    connect(_communicatorRcvTrafficShip, SIGNAL(signalReceiverConnectionStatus(bool)), ui->wRcvSignal_2, SLOT(setTraficSignal(bool)));
    connect(_communicatorSend, SIGNAL(signalReceiverConnectionStatus(bool)), ui->wTransSignal, SLOT(setTraficSignal(bool)));

    connect(this, SIGNAL(connectionStatus(bool)),ui->wDbWriteSignal, SLOT(setTraficSignal(bool)));

    connect(this, SIGNAL(sendTransmitterKey(QString)), _communicatorSend, SLOT(setTransmitterKey(QString)));
    ui->pb_StopStore->setEnabled(false);
}

UserDatabase::~UserDatabase()
{
    dbWriter->closeDB();
    delete ui;
}

void UserDatabase::userListUpdate(QMap<QString, UserInfo> list)
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

//void UserDatabase::on_pbConnect_clicked()
//{
//    if(setServer)
//    {
//        bool connected = dbWriter->openDB(serverUrl);
//        if(connected)
//        {
//            qDebug()<<"Database is connected.";
//        }
//        else
//        {
//            qDebug()<<"Database connection error";
//        }
//        emit connectionStatus(connected);
//    }
//}

void UserDatabase::on_pbImport_clicked()
{
    dbWriter->importDataFromServer();
}

void UserDatabase::on_pbStore_clicked()
{    
    connect(_communicatorRcvOwnShip,SIGNAL(onTpMsgReceived(QByteArray)),dbWriter,SLOT(receivedOwnShipFromServer(QByteArray)));
    connect(_communicatorRcvTrafficShip,SIGNAL(onTpMsgReceived(QByteArray)),dbWriter,SLOT(receivedTrafficShipFromServer(QByteArray)));

    ui->pbStore->setEnabled(false);
    ui->pb_StopStore->setEnabled(true);
//     qDebug()<<"Application is ready to store sensor data.";

//            dbClients.createJsonForTesting();
}

void UserDatabase::receiveServerUrl(int id, QString url_os, QString url_ts)
{    
    osServerUrl = url_os;
    tsServerUrl = url_ts;
    qDebug()<<"Here it is"<<osServerUrl<<tsServerUrl;
    if(osServerUrl.data() && tsServerUrl.data())
    {
        setServer = true;
    }
    else
    {
        setServer = false;
    }
    if(setServer)
    {
        bool connected = dbWriter->openDB(id, osServerUrl,tsServerUrl);
        if(connected)
        {
            qDebug()<<"Database is connected.";
        }
        else
        {
            qDebug()<<"Database connection error";
        }
        emit connectionStatus(connected);
    }
}

void UserDatabase::on_usersList_activated(int index)
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

void UserDatabase::displayMsg(QByteArray array)
{
    ui->te_DisplayMsg->append(array);
}

void UserDatabase::on_pb_StopStore_clicked()
{
    disconnect(_communicatorRcvOwnShip,SIGNAL(onTpMsgReceived(QByteArray)),dbWriter,SLOT(receiveJsonFromServer(QByteArray)));
    disconnect(_communicatorRcvTrafficShip,SIGNAL(onTpMsgReceived(QByteArray)),dbWriter,SLOT(receivedTrafficShipFromServer(QByteArray)));

    ui->pbStore->setEnabled(true);
    ui->pb_StopStore->setEnabled(false);
    qDebug()<<"Application is not storing sensor data anymore.";
}

void UserDatabase::on_pbSend_clicked()
{
//    connect(dbWriter, SIGNAL(sendJsonToServer(QByteArray)), _communicatorSend, SLOT(sendTpMsg(QByteArray)));
}

void UserDatabase::on_pbStoreDummy_clicked()
{
    dbWriter->createOwnShipDataJsonForTesting();
}


void UserDatabase::on_pbStoreDummy_2_clicked()
{
    dbWriter->createTrafficShipDataJsonForTesting();
}
