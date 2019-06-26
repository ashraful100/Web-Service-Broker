#include "multishipaisdata.h"
#include "ui_multishipaisdata.h"

MultiShipAISData::MultiShipAISData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiShipAISData)
{
    ui->setupUi(this);

    _communicator =new Communicator("MSAis");
    connect(this, SIGNAL(serverSettings(QString,QString)), _communicator, SLOT(setAmqpServer(QString,QString)));

    for(int i=0;i<6;i++)
    {
        DisplayReceivedAISData  *_aisWidget =new DisplayReceivedAISData(this);
        _aisWidget->setShipID(i+1);

        ui->swAis->addWidget(_aisWidget);
        connect(this, SIGNAL(sendServerAddress(QString,QString)), _aisWidget, SIGNAL(setServerAddress(QString,QString)));
        connect(this, SIGNAL(passUsersName(QMap<QString, UserInfo>)),_aisWidget,SLOT(userListUpdate(QMap<QString, UserInfo>)));
        connect(_aisWidget,SIGNAL(sendDataToCommunicator(QByteArray)),_communicator,SIGNAL(sendTpMsg(QByteArray)));
        connect(_aisWidget,SIGNAL(sendTransmitterKey(QString)),_communicator, SLOT(setTransmitterKey(QString)));
        connect(_aisWidget, SIGNAL(sendConnectionStatus(bool)),this, SIGNAL(sendConnectionStatus(bool)));

        aisButtongroup.addButton(getShipButton(i+1),i);
    }
    connect(&aisButtongroup,SIGNAL(buttonClicked(int)),ui->swAis,SLOT(setCurrentIndex(int)));
}

MultiShipAISData::~MultiShipAISData()
{
    delete ui;
}

QPushButton *MultiShipAISData::getShipButton(int id)
{    
    return  findChild<QPushButton*>(QString("pbShip%1").arg(id));
}
