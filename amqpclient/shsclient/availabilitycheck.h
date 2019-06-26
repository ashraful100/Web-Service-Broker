#ifndef AVAILABILITYCHECK_H
#define AVAILABILITYCHECK_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include <QTimer>
#include "amqpclient/receiver.h"
#include "amqpclient/transmitter.h"

#include "amqpclient/shsclient/xmlparser.h"
#include "amqpclient/shsclient/readgeneral.h"
#include "amqpclient/shsclient/readais.h"
#include "amqpclient/shsclient/radar.h"

#include "amqpclient/shsclient/nmeacomposer.h"

#include "amqpclient/shsclient/amqpserversettings.h"

//AIS
//#include "aisdecoderlib.h"


//!
//! \brief The Availability class constantly checks if ships are avialble at amqp queue
//! \ingroup shsclient
//!
class Availability : public QMainWindow
{
    Q_OBJECT

public:
    explicit Availability(QString AppId, int shsId, QWidget *parent = 0);
    ~Availability();

signals:

    void signalDataAvailable(int shsId,int availabilityId,QString &server,QString &name);

public slots:

    void purgeAllQueues();
    void connectClient();
    void disconnectClient();
    void resetClient();    
    void setServerSettingsObj(AmqpServerSettings *_amqpServerSettings);   
    void setAmqpServer(QString url, QString name);
    void slotOnOsDataSourceChanged(int sourceId); //1= SHS 3 = AIS

private slots:
    void updateConnectionSettings();
    void createClients();
    void slotShipDataOnAvailable(QString &shipName);

    void dataTimeOut();
    void attemptReconnect();
    void slotXmlShipData(QString &shipName);
    //void slotAisShipData(EcAISTargetInfo *ti, int shsId);
private:
    QString amqpUrl,queue,fanout;
    QString qAis;
    QString foAis;
    QStringList listShsFanoutName;
    QTimer *dataStreamTimer,*reconnectTimer;

    // AMQP Receivers
    QAMQP::Receiver *_receive;
    QAMQP::Receiver *_receiveAis;


    XmlParser *_xmlParser ;
    bool connectionStatus;

    AmqpServerSettings *_amqpServerSettings;
    QString appId;
    int shsId;
    QLabel *_label;
    QTimer timer;
    QString serverName;
    int source;
    //AIS decoder
   // AISDecoderLib *_aisDecoder;
    QString dataInfo;


};

#endif // AVAILABILITYCHECK_H
