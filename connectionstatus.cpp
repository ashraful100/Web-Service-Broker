#include "connectionstatus.h"
#include "ui_connectionstatus.h"

void ConnectionStatus::setTraficSignal(bool _status)
{
    if(_status)
    {
        status=true;
    }
    else{
        status=false;
    }
    this->update();
}

ConnectionStatus::ConnectionStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionStatus)
{
    ui->setupUi(this);
    connect(this, SIGNAL(connectionStatus(bool)), this, SLOT(setTraficSignal(bool)));
}

ConnectionStatus::~ConnectionStatus()
{
    delete ui;
}

void ConnectionStatus::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(!status)
    {
        painter.setBrush(QBrush(Qt::gray));
    }
    else
    {
        painter.setBrush(QBrush(Qt::green));
    }
    painter.drawEllipse(5,5,15,15);

}
