#ifndef VIS_H
#define VIS_H

#include <QObject>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QGeoCoordinate>
#include <QJsonArray>

#include "model/voyage/dataclasses/voyagedetails.h"
#include "model/voyage/dataclasses/waypoint.h"
#include "model/voyage/parser/xmlvoyageparser.h"

class VIS : public QObject
{
    Q_OBJECT
public:
    explicit VIS(QObject *parent = 0);
    ~VIS();
    enum REQ_TYPE{
        REQ_INVALID =10,
        REQ_POST_VOY,
        REQ_GET_VOY,
        REQ_POST_MSG

    };

signals:

    void signalVoyageFromVis(QByteArray);
    void signalPostVoyageToVis(QByteArray);

public slots:

    void slotRequestVoyage(QByteArray reqData);
    int getMmsiFromVisReq(QByteArray visReq);

    void slotPostVoyage(int mmsi);
    void postToVIS();
    void postSingle(int mmsi);
private slots:
    int getRequestType(QByteArray visReq);
    void respondToGetVoyage(int mmsi);

    QString getVoyageFromMessage(QJsonObject payload);
    QString parseVoyageFromRemote(QByteArray raw);
    void postVoyageLocallyAndRespond(QByteArray postVoyageMsg, int mmsi);
private:

    QByteArray createVoyage(int mmsi);

    QMap<int,int> mmsiMap;
    XmlVoyageParser parser;

    QMap<int,QString> voyageMap;
};

#endif // VIS_H
