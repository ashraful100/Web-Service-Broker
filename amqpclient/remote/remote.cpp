#include "remote.h"

Remote::Remote(QString _appId,QObject *parent) : QObject(parent)
  ,appId(_appId)
{

 connectionStatus=false;

}

Remote::~Remote()
{
    if(connectionStatus){
    delete _sendControl;
    delete _receiveControl;
    }
}


void Remote::connectClient(){

    if(connectionStatus)
        return;

    QString qReceiveCntrl = QString("Q_DEMO_%1").arg(appId);
    QString foReceiveCntrl = "FO_DEMO_REMOTE";
    QString foSendFeedback = "FO_DEMO_FEEDBACK";

    _sendControl= new QAMQP::Transmitter(this);
    _sendControl->setFanoutName(foSendFeedback);
    _sendControl->setFanoutFlag(true);
    _sendControl->setServerAddress(amqpUrl);
    _sendControl->updateSettings();
    connect(this,SIGNAL(signalSendMsgToRemote(QByteArray)),_sendControl,SLOT(sendMessageArray(QByteArray)));
    //connect(_sendControl,SIGNAL(signalConnectionStatus(bool)),ui->indShs,SLOT(setChecked(bool)));


    _receiveControl = new QAMQP::Receiver(this);
    _receiveControl->setQueueName(qReceiveCntrl);
    _receiveControl->setFanoutName(foReceiveCntrl);
    _receiveControl->setServerAddress(amqpUrl);
    _receiveControl->updateSettings();
    connect(_receiveControl,SIGNAL(onMessageReceived(QByteArray)),this,SIGNAL(signalOnRemoteMessageReceived(QByteArray)));
   // connect(_receiveControl,SIGNAL(signalConnectionStatus(bool)),ui->indCmd,SLOT(setChecked(bool)));

    connectionStatus =true;

}

void Remote::disconnectClient(){

    if(!connectionStatus)
        return;

    _sendControl->closeConnection();
    _receiveControl->slotDisconnected();
    connectionStatus =false;

}


void Remote::resetClient(){

  disconnectClient();
  connectClient();
}


void Remote::setAmqpServer(QString url, QString name)
{
    amqpUrl = url;
    server = name;
    resetClient();
}

