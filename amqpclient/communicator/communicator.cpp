#include "communicator.h"
#include <QJsonDocument>

/** @class Communicator
 *  @brief The Communicator class manages the communication between the ANS and the SOC. It is a standard class that can be used both as sender and receiver
 *  and is managed by messaging keys on the Rabbit MQ exchanges and queues
 */

//!
//! \brief Constructos the Communicator object
//! \param AppId indicates the application the Communicator is used in. Can contain Values "ANS" or "SOC"
//! \param statusIncoming is a pointer to the instance of the ConnectionStatusManager class as incoming data console
//! \param statusOutgoing is a pointer to the instance of the ConnectionStatusManager class as outgoing data console
//! \param parent to the Communicator which in this case is the ConnectionManager class
//!
Communicator::Communicator(QString AppId, QObject *parent) :
    QObject(parent)
  , appId(AppId)
{
    connectionStatus =false;   

    //Keys for source and destination of queues
    //Transmitter key refers to the destination app

    //Receiver key refers to the source app
    //transmitterKey ="5a0ecc330e76430e1c019369";
//    transmitterKey ="KEY_NODE";
    receiverKey = "K_QT";

    //createClients();

}
//!
//! \brief Deletes the communicator object
//!
Communicator::~Communicator()
{

}

void Communicator::setTransmitterKey(QString key)
{
    transmitterKey =key;
    qDebug()<<"From communicator"<<transmitterKey;
    if(connectionStatus)
        _sendTP->setMsgKey(transmitterKey);

}

//!
//! \brief Sets the connection identifiers for the Communicator. There are two identifiers used for the Communicator.
//!
//! \arg 1. Ship ID that describs to which slot in the ANS and SOC the communication happens
//! \arg  2. System ID that describes the different systems running the same SHIp source. This enables duplication of ANS and SOC systems with single data source for ship
//!
//! \param shipId is the Ship ID slot to which the ANS or the SOC is connected to
//! \param systemId is the System number on which the ANS or SOC is running
//!
void Communicator::setConnectionIdentifiers(int shipId,int systemId){



}
//!
//! \brief Connects the communicator to the identifiers used in \ref Communicator::setConnectionIdentifiers(int shipId,int systemId)
//!
void Communicator::connectClient(){

    disconnectClient();
    updateConnectionSettings();
    createClients();
    connectionStatus =true;
}
//!
//! \brief Disconnects the communicator to the identifiers used in \ref Communicator::setConnectionIdentifiers(int shipId,int systemId).
//! All the AMQO objects are disconnected and deleted
//!
void Communicator::disconnectClient()
{
    if(connectionStatus)
    {
        _receiveTP->closeConnection();
        _sendTP->closeConnection();

        _receiveTP->deleteLater();
        _sendTP->deleteLater();

        connectionStatus =false;

    }
}
//!
//! \brief Resets the connection
//!
void Communicator::resetClient(){

    disconnectClient();
    connectClient();
}

//!
//! \brief Updates the AMQP parameters for the URL , Exchange and Queue names before connecting
//!
void Communicator::updateConnectionSettings()
{
    if(!connectionStatus)
    {
        if(appId.contains("DBRCVO")){
//            qTP = QString("Q_MUH_DB");
//            foTP = QString("FO_CANBUS_MUH");
//            receiverKey = "K_DB";

            qTP = QString("Q_MUH_DB_SEN");
            foTP = QString("FO_MUH_REMOTE"); // foTP = QString("FO_MUH_CTRL");
            receiverKey = "K_MUH_DB_SEN";
            qDebug()<<qTP<<foTP<<receiverKey;
        } else{
            qTP = QString("Q_QT_%1").arg(appId);
            foTP =QString("EMSN_DATA_SIM");
        }

        qDebug()<<"Connected with "<<qTP<<foTP<<receiverKey;
    }
}
//!
//! \brief Creates the AMQP receiver objects based on the settings
//! \param _receiver Pointer to the Receiver object
//! \param q is the QUEUE name
//! \param fo is teh EXCAHGE name
//! \sa \ref QAMQP::Receiver
//!
void Communicator::createReceivers( QAMQP::Receiver **_receiver,QString &q,QString &fo)
{
    (*_receiver) = new QAMQP::Receiver(this);
    (*_receiver)->setQueueName(q,receiverKey);
    (*_receiver)->setFanoutName(fo);
    (*_receiver)->setServerAddress(amqpUrl);
    (*_receiver)->updateSettings();
}

//!
//! \brief Creates the AMWP transmitter objects based on the settings
//! \param _transmitter is the piointer to the Transmitter object
//! \param fo is the Echnage name
//! \sa QAMQP::Transmitter
//!
void Communicator::createTransmitters( QAMQP::Transmitter **_transmitter,QString &fo)
{
    (*_transmitter) = new QAMQP::Transmitter(this);
    (*_transmitter)->setMsgKey(transmitterKey);
    (*_transmitter)->setFanoutName(fo,"direct");
    (*_transmitter)->setFanoutFlag(true);
    (*_transmitter)->setServerAddress(amqpUrl);
    (*_transmitter)->updateSettings();
}

//! \brief Creates the Transmitter and Receiver objects needed in the Communicator.
//! A receiver and transmitter is created for each type of data viz.
//! \arg TP Track Pilot Data
//! \arg WR Harsh Weather Data
//! \arg CA Collision Avaoidance Data
//! \arg VO Voyage related data
//!

void Communicator::createClients()
{
    createReceivers(&_receiveTP,qTP,foTP);
    createTransmitters(&_sendTP,foTP);

    connect(_receiveTP,SIGNAL(onMessageReceived(QByteArray)),this,SIGNAL(onTpMsgReceived(QByteArray)));
    connect(_receiveTP,SIGNAL(signalConnectionStatus(bool)),this, SIGNAL(signalReceiverConnectionStatus(bool)));
    connect(_receiveTP,SIGNAL(signalConnectionStatus(bool)),this, SIGNAL(signalTransmitterConnectionStatus(bool)));
    connect(this,SIGNAL(sendTpMsg(QByteArray)),_sendTP,SLOT(sendMessageArray(QByteArray)));

}
//!
//! \brief Sets the status of the VOYAGE Transmitter data connection
//! \param status is the connected or disconnected state
//!
void Communicator::slotVoyageSendStatus(bool status)
{

}

//!
//! \brief Sets the status of the VOYAGE Receiver data connection
//! \param status is the connected or disconnected state
//!
void Communicator::slotVoyageReceiveStatus(bool status)
{

}
//!
//! \brief Purges all the queues that the communicator is connected to and has access to.
//! This is used to unclog the RQ server
//!
void Communicator::purgeAllQueues()
{
    if(connectionStatus)
    {
        _receiveTP->purgeQ();
    }
}

//!
//! \brief Sets the interdependency to the AmqpServerSettings object.
//! AmqpServerSettings  stores and manages the AMQP server settings
//! \param _amqpServerSettings is the Pointer to the AmqpServerSettings object associated with this Communicator
//!
void Communicator::setServerSettingsObj( AmqpServerSettings *_amqpServerSettings){

    this->_amqpServerSettings =_amqpServerSettings;
    connect(_amqpServerSettings,SIGNAL(signalOnServerChanged(QString,QString)),this,SLOT(setAmqpServer(QString,QString)));
}
//!
//! \brief Sets the AMQP server address and the name to which the Communicator connects to
//! \param url
//! \param name
//!
void Communicator::setAmqpServer(QString url, QString name)
{
    amqpUrl = url;
    server = name;    
    resetClient();
}

