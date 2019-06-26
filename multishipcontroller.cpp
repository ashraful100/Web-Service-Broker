#include "multishipcontroller.h"
#include "ui_multishipcontroller.h"

MultiShipController::MultiShipController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiShipController)
{
    ui->setupUi(this);

    _communicator =new Communicator("MSC");
    connect(this, SIGNAL(serverSettings(QString,QString)), _communicator, SLOT(setAmqpServer(QString,QString)));

        for(int i=0;i<10;i++)
        {
            CalculateShipLocation  *_shipWidget =new CalculateShipLocation(this);
            _shipWidget->setId(i+1);
            ui->swShips->addWidget(_shipWidget);

            shipButtongroup.addButton(getShipButton(i+1),i);
            connect(_shipWidget, SIGNAL(sendTransmitterKey(QString)),_communicator, SLOT(setTransmitterKey(QString)));
            connect(_shipWidget, SIGNAL(sendDataToCommunicator(QByteArray)),_communicator,SIGNAL(sendTpMsg(QByteArray)));
            connect(this, SIGNAL(passUsersName(QMap<QString, UserInfo>)),_shipWidget,SLOT(userListUpdate(QMap<QString, UserInfo>)));
        }
        connect(&shipButtongroup,SIGNAL(buttonClicked(int)),ui->swShips,SLOT(setCurrentIndex(int)));
}

MultiShipController::~MultiShipController()
{
    delete ui;
}

QPushButton *MultiShipController::getShipButton(int id)
{
    return  findChild<QPushButton*>(QString("pbShip%1").arg(id));
}
