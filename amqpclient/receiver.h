#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QRunnable>
#include <QDebug>
#include <QTime>

#include "qamqp/amqp.h"
#include "qamqp/amqp_queue.h"


namespace QAMQP
{

//!
//! \brief The Receiver class provides an interface to receive AMPQ data from the server
//! \ingroup amqpclient
//!
class Receiver : public QObject
{
    Q_OBJECT

    typedef QObject super;

public:
    explicit Receiver(QObject* parent=0)
        : super(parent)
    {
        // Create AMQP client
        client = new QAMQP::Client(this);
        key="";

    }


    void closeConnection()
    {        
        client->close();
    }

    bool clientIsConnected()
    {
        return client->isConnected();
    }

    void setQueueName(QString queueName)
    {
        this->queueName =  queueName;

    }

    void setQueueName(QString queueName,QString key)
    {
        this->queueName =  queueName;
        this->key =key;
    }

    void setServerAddress(QString servAdd)
    {
        this->servAdd =  servAdd;
    }

    void setFanoutName(QString fanoutName)
    {
        this->fanoutName =  fanoutName;
    }

    void updateSettings()
    {
        if(client->isConnected())
        {
            client->close();
        }
        connect(client,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(client,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

        client->open(QUrl(servAdd));      

        // Create an exclusive queue
        queue_ = client->createQueue();
        queue_->declare(queueName,Queue::Durable);

        connect(queue_, SIGNAL(declared()), this, SLOT(declared()));
        connect(queue_, SIGNAL(messageReceived(QAMQP::Queue*)), this, SLOT(newMessage(QAMQP::Queue*)));

    }
signals:

    void onMessageReceived(QByteArray receivedMsg);
    void connected();
    void disconnected();
    void signalConnectionStatus(bool connected);


public slots:

    void slotConnected()
    {
        emit connected();
        emit signalConnectionStatus(true);
    }

    void slotDisconnected()
    {
        purgeQ();
        queue_->unbind(fanoutName,key);        
        emit disconnected();
        emit signalConnectionStatus(false);
    }

    void purgeQ()
    {
        queue_->purge();
    }

    void deleteQ(){
         queue_->remove();
    }

protected slots:
    void declared()
    {
        queue_->purge();
        queue_->bind(fanoutName,key);
        queue_->consume();
    }

    void newMessage(QAMQP::Queue* queue_)
    {
        QAMQP::MessagePtr message = queue_->getMessage();
        queue_->ack(message);
//        qDebug()<<"Receive";
        emit onMessageReceived(message->payload);
    }


private:
    QAMQP::Queue* queue_;
    QString queueName,fanoutName,servAdd,key;
    QAMQP::Client* client;

};



}

#endif // RECEIVER_H
