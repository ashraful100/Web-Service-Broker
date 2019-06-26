#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QWidget>
#include <QThread>

#include <QButtonGroup>
#include "dataviewer.h"
#include "amqpclient/shsclient/shsclient.h"
#include "amqpclient/shsclient/shsdata.h"
#include "aisdecoder/trafficshipdata.h"
#include "aisdecoder/aisdecoder.h"
#include "amqpclient/shsclient/tsdataswitch.h"
#include "amqpclient/communicator/communicator.h"
#include "view/datarategraph/connectionstatus.h"
#include "view/datarategraph/connectionstatusmanager.h"
#include "amqpclient/shsclient/connector.h"
#include "view/shipinfo/shipinfowidget.h"
#include "view/shipinfo/shipinfowidgetlite.h"
#include "amqpclient/remote/remoteid.h"


namespace Ui {
class ConnectionManager;
}

//!
//! \brief The ConnectionManager class enables settings of connections between ship handling simulator and ANS respecetively SOC
//! \ingroup shsclient
//!
class ConnectionManager : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionManager(QString AppId,int shipId, int systemId, CallBack *_callBack, QWidget *parent = 0);
    ~ConnectionManager();


    ShsClient *getShsClient();
    Communicator *communicator() const;

    TrafficShipData *trafficShipData() const;
    ShsClient *shsClient() const;
    Connector *connector() const;


    ShipInfoWidget *shipInfoWidget() const;
    ShipInfoWidgetLite *shipInfoWidgetLite() const;

public slots:
    void parseRemoteControlMessage(QByteArray remoteMsgAry);
    void slotOnOsDataSourceChanged(int sourceId);
signals:

    void signalAisMessageReceived(QByteArray);
    void signalGenMessageReceived(QByteArray);
    void signalTtmMessageReceived(QByteArray);
    void signalShsMessageReceived(QByteArray);
    void signalOsData(QMap<QString,QString> osDataMap);
    void signalTsData(QMap<QString,QString> tsDataMap);
    void signalOnTsDataSourceChanged(int sourceId);

    void signalOnAisDataSourceChanged(int sourceId);
    void signalOnConnectorActivated(bool connected, int shipIndex, int userIndex);
    void signalDataAvailable(int shsId,int availabilityId,QString &server,QString &name);
    void signalOnAisRangeChanged(double rangeInNM);

private slots:

    void connectClient(bool checked);
    void slotOnConnectorActivated(bool connected, int shipIndex, int userIndex);

    void processRemoteControlMsg(QStringList &remoteMsgList);
    void parseTypeControllerMessage(QStringList &controlMsg);
private:

    Ui::ConnectionManager *ui;
    ShsClient *_shsClient;
    ShsData *_shsData;
    TrafficShipData *_trafficShipData;
    AisDecoder  *_aisDecoder;
    TsDataSwitch *_tsDataSwitch;

    Communicator *_communicator;
    Connector *_connector;

    QButtonGroup buttonGroup;
    ShipInfoWidget *_shipInfoWidget;
    ShipInfoWidgetLite *_shipInfoWidgetLite;
    QString appId;
    int id;

    QThread shsDataThread;





};


#endif // CONNECTIONMANAGER_H
