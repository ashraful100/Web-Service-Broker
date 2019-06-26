#ifndef SHSCLIENT_H
#define SHSCLIENT_H

#include <QMainWindow>
#include <QCloseEvent>
#include "amqpclient/receiver.h"
#include "amqpclient/transmitter.h"

//#include "amqpclient/shsclient/xmlparser.h"
//#include "amqpclient/shsclient/readgeneral.h"
//#include "amqpclient/shsclient/readais.h"
//#include "amqpclient/shsclient/radar.h"

#include "amqpclient/shsclient/nmeacomposer.h"
#include "amqpclient/shsclient/amqpserversettings.h"
//#include "view/datarategraph/connectionstatusmanager.h"
#include "dataviewer.h"


class ShsClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit ShsClient(QString AppId, QString queue, /*ConnectionStatusManager *conStatusManager,*/QWidget *parent = 0);
    ~ShsClient();


//    ReadGeneral *readGeneral() const;
//    void setReadGeneral(ReadGeneral *readGeneral);

//    ReadAis *readAis() const;
//    void setReadAis(ReadAis *readAis);

//    ReadTtm *radar() const;
//    void setRadar(ReadTtm *radar);

//    NmeaComposer *nmeaComposer() const;
//    void setNmeaComposer(NmeaComposer *nmeaComposer);

//    XmlParser *xmlParser() const;
//    void setXmlParser(XmlParser *xmlParser);

signals:

    void signalAisMessageReceived(QByteArray);
//    void signalGenMessageReceived(QByteArray);
//    void signalTtmMessageReceived(QByteArray);
//    void signalShsMessageReceived(QByteArray);
    void signalOnOsDataSourceChanged(int sourceId);
    void signalReceiverConnectionStatus(bool);

public slots:

    void purgeAllQueues();
    void connectClient();
    void disconnectClient();
    void resetClient();        
    void setConnectionIdentifiers(int shipId, int systemId);
    void setAmqpServer(QString url, QString name);

//    void connectShipDataToDataViewer(bool connected);
private slots:
    void updateConnectionSettings();
    void createClients();


private:
    QString amqpUrl,
    foShs,foAis,foNav, foTtm,foCmd,fanoutNameAscSend,fanoutNameAscReceive,fanoutNameScc,
    qShs,qAis,qNav,qTtm,qCmd,qNameAscSend,qNameAscReceive,qNameScc;

    // AMQP Receivers
//    QAMQP::Receiver *_receiveShs;
    QAMQP::Receiver *_receiveAis;
//    QAMQP::Receiver *_receiveNav;
//    QAMQP::Receiver *_receiveTtm;
//    QAMQP::Transmitter *_sendCmd;

    //Reader objects
//    ReadGeneral *_readGeneral;
//    ReadAis *_readAis;
//    ReadTtm *_radar;
//    NmeaComposer *_nmeaComposer;
//    XmlParser *_xmlParser ;

    // Indicator
    QString style_off,style_on;

    int systemId,shipId;

    bool connectionStatus;
    QString appId;
//    ConnectionStatusManager *_conStatusManager;
    QString server;



};

#endif // SHSCLIENT_H
