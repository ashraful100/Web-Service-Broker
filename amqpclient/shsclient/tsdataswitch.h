#ifndef TRAFFICDATASOURCESELECTOR_H
#define TRAFFICDATASOURCESELECTOR_H

#include <QObject>
#include <QMap>

//!
//! \brief The TsDataSwitch class switched Id SHS or AIS data is received for TrafficShip
//! \ingroup shsclient
//!
class TsDataSwitch : public QObject
{
    Q_OBJECT
public:
    explicit TsDataSwitch(QObject *parent = 0);
    ~TsDataSwitch();

signals:
     void signalTsData(QMap<QString,QString>);

public slots:
    void OnTsDataSourceChanged(int id);

    void onTsAisDataReceived(QMap<QString, QString> tsAisData);
    void onTsShsDataReceived(QMap<QString, QString> tsShsData);
private:
    int targetId;
};

#endif // TRAFFICDATASOURCESELECTOR_H
