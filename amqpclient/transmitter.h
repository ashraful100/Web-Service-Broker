#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QObject>
#include <QRunnable>
//#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QTimer>
#include <QDateTime>
#include <QJsonDocument>

#include "qamqp/amqp.h"
#include "qamqp/amqp_exchange.h"
#include "qamqp/amqp_queue.h"

namespace QAMQP
{


//!
//! \brief The Transmitter class provides an interface to transmit AMQP data to the server
//! \ingroup amqpclient
//!
class Transmitter : public QObject
{
    Q_OBJECT

    typedef QObject super;

public:

    explicit Transmitter(QObject *parent=0)
        : super(parent)
    {

        client = new QAMQP::Client(this);

        key ="";
        fanoutType ="fanout";

    }


    void sendMessageContent(QString sendMsg)
    {
        sendMessage(sendMsg);
    }


    void closeConnection()
    {
        client->close();
        client->closeChannel();

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
        this->key =  key;
    }

    void setMsgKey(QString key)
    {
        this->key =  key;
//        qDebug() << this << "Transmitter key is " << key;
    }

    void setServerAddress(QString servAdd)
    {
        this->servAdd =  servAdd;
    }

    void setFanoutFlag(bool fanoutFlag)
    {
        this->fanoutFlag = fanoutFlag;
    }

    void setQueueFlag(bool queueFlag)
    {
        this->queueFlag = queueFlag;
    }

    void setFanoutName(QString fanoutName)
    {
        this->fanoutName =  fanoutName;
    }

    void setFanoutName(QString fanoutName,QString fanoutType)
    {
        this->fanoutName =  fanoutName;
        this->fanoutType =  fanoutType;
    }

    void setFanoutType(QString fanoutType)
    {
        this->fanoutType =  fanoutType;
    }

    void updateSettings(){

        if(client->isConnected())
        {
            client->close();
        }
        connect(client,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(client,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));



        client->open(QUrl(servAdd));

        if(fanoutFlag)
        {
            exchange_ =  client->createExchange(fanoutName);
            exchange_->declare(fanoutType,Exchange::Durable);
        }
        else
        {
            exchange_ =  client->createExchange();
        }

    }
signals:

    void signalConnected();
    void signalDisconnected();
    void signalConnectionStatus(bool connected);

public slots:

    void slotConnected()
    {
        emit signalConnected();
        emit signalConnectionStatus(true);
    }
    void slotDisconnected()
    {
        emit signalDisconnected();
        emit signalConnectionStatus(false);
    }

    void sendMessageArray(QByteArray sendMsg)
    {        
//        qDebug() << "sendMsg";
//        qDebug()<<QJsonDocument::fromJson(sendMsg) ;
        exchange_->publish(sendMsg,key);
//        qDebug() << this << "From transmitter: " << key;
    }

protected slots:

    void sendMessage(QString sendMsg)
    {
        exchange_->publish(sendMsg,key);
    }

private:

    QString sendMsg_;
    QAMQP::Exchange* exchange_;
    QString queueName,fanoutName,servAdd,key,fanoutType;
    bool fanoutFlag,queueFlag;
    QAMQP::Client* client;
    QAMQP::Queue* queue_;

};



}

#endif // TRANSMITTER_H
