#ifndef SHSDATA_H
#define SHSDATA_H

#include <QObject>
#include <QMap>
#include <QGeoCoordinate>
#include <QTime>

//!
//! \brief The ShsData class handles/parses the incoming SHS data
//! \ingroup shsclient
//!
//! *'*
class ShsData : public QObject
{
    Q_OBJECT
public:
    explicit ShsData(QObject *parent = 0);

signals:

    void osObject(QMap<QString,QString> ownship);
    void tsObject(QMap<QString,QString> trafficship);


public slots:

    void onShsRead(QMap<QString,QString> ShsData);
    void setShipId(int shipId);

private slots:
    void setTrafShipData(QMap<QString, QString> ShsData, QMap<QString, QString> osMap);
    void setOwnShipData(QMap<QString, QString> ShsData);

private:

    int engineId;
    int rudderId;
    int thrusterId;
    int shipId;

    QMap<QString,QString> ownship;
    QMap<QString,QString> trafficship;

};

#endif // SHSDATA_H
