#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("websbi.ico"));
    this->setWindowTitle("CML WEB Service Broker Interface (WEBSBI) V9");

    shipData = new MultiShipController(this);
    aisData = new MultiShipAISData(this);
    routeArray = new RouteArray(this);
    userDetails = new UsersDetails(this);
    userDatabase = new UserDatabase(this);
    sensorInterface = new SensorInterface(this);
    _filterUserData = new FilterUserData(this);

    ui->swSubPub->addWidget(aisData);
    ui->swSubPub->addWidget(shipData);
    ui->swSubPub->addWidget(routeArray);
    ui->swSubPub->addWidget(userDetails);
    ui->swSubPub->addWidget(userDatabase);
    ui->swSubPub->addWidget(sensorInterface);

    _communicator =new Communicator("MW");

    connect(ui->subServer, SIGNAL(signalOnServerChanged(QString,QString)),aisData, SIGNAL(sendServerAddress(QString,QString)));
    connect(aisData, SIGNAL(sendConnectionStatus(bool)), ui->wAISSignal,SLOT(setTraficSignal(bool)));
    connect(ui->webServer,SIGNAL(signalOnServerChanged(QString,QString)),aisData, SIGNAL(serverSettings(QString,QString)));
    connect(ui->webServer,SIGNAL(signalOnServerChanged(QString,QString)),shipData, SIGNAL(serverSettings(QString,QString)));
    connect(ui->webServer,SIGNAL(signalOnServerChanged(QString,QString)),routeArray, SIGNAL(serverSettings(QString,QString)));
    connect(ui->webServer, SIGNAL(signalOnServerChanged(QString,QString)),_communicator, SLOT(setAmqpServer(QString,QString)));
    connect(_communicator, SIGNAL(signalReceiverConnectionStatus(bool)), ui->wWebSignal, SLOT(setTraficSignal(bool)));
    connect(ui->webServer, SIGNAL(signalOnServerChanged(QString,QString)),sensorInterface,SLOT(setCommunicatorSettings(QString,QString)));

    connect(ui->dbServerRead, SIGNAL(signalOnServerChanged(int,QString,QString)),sensorInterface, SLOT(checkDatabaseConnection(int,QString,QString)));
    connect(sensorInterface, SIGNAL(connectionStatus(bool)), ui->wDatabaseSignalRead, SIGNAL(connectionStatus(bool)));

//    connect(_filterUserData, SIGNAL(transmitterKey(QString)),_communicator, SLOT(setTransmitterKey(QString)));
    connect(_communicator,SIGNAL(onTpMsgReceived(QByteArray)),_filterUserData,SLOT(checkTypeOfData(QByteArray)));
    connect(_filterUserData,SIGNAL(sendDataToUserDetails(QByteArray)),userDetails,SLOT(onMessageReceivedFromWeb(QByteArray)));
    connect(_filterUserData, SIGNAL(sendDataToSensorInterface(QByteArray)),sensorInterface,SLOT(onWebDataRecived(QByteArray)));
//    connect(sensorInterface, SIGNAL(sendSensorData(QByteArray)),_communicator,SIGNAL(sendTpMsg(QByteArray)));

    //    {"appId":"MUH","dataId":"0","reqId":2,"userId":"5b7e778faea86610b46ae575","userName":"guest"}

    connect(userDetails, SIGNAL(sendUsersList(QMap<QString, UserInfo>)),routeArray, SLOT(userListUpdate(QMap<QString, UserInfo>)));
    connect(userDetails, SIGNAL(sendUsersList(QMap<QString, UserInfo>)),aisData, SIGNAL(passUsersName(QMap<QString, UserInfo>)));
    connect(userDetails, SIGNAL(sendUsersList(QMap<QString, UserInfo>)),shipData, SIGNAL(passUsersName(QMap<QString, UserInfo>)));
    connect(userDetails, SIGNAL(sendUsersList(QMap<QString, UserInfo>)),userDatabase, SLOT(userListUpdate(QMap<QString, UserInfo>)));

    buttonGroup.addButton(ui->pbAis,0);
    buttonGroup.addButton(ui->pbShipManual,1);
    buttonGroup.addButton(ui->pbRoute,2);
    buttonGroup.addButton(ui->pbUser,3);
    buttonGroup.addButton(ui->pbUserDatabase,4);
    buttonGroup.addButton(ui->pbSensorInterface,5);

    connect(&buttonGroup,SIGNAL(buttonClicked(int)),ui->swSubPub,SLOT(setCurrentIndex(int)));
    buttonGroup.setExclusive(true);
    buttonGroup.button(3)->click();
}

MainWindow::~MainWindow()
{
    delete ui;
}

