#ifndef READGENERAL_H
#define READGENERAL_H




#include <QObject>
#include <QGeoCoordinate>
#include <QApplication>
#include <QMutex>
#include <QTime>

#include "nmeaclient/nmeatoxml.h"


//!
//! \brief The ReadGeneral class reads the general SHS data from Ship Handling Simulator
//! \ingroup shsclient
//!
class ReadGeneral : public QObject
{
    Q_OBJECT
public:

    explicit ReadGeneral(QObject *parent = 0);

    ~ReadGeneral();




signals:


    void signalSendNmeaDataMap(QMap<QString,QString> &nmeaDataMap);

    void signalXmlFromNmea(QByteArray xml);



public slots:
    void slotSetOsAisdata(QMap<QString,QString> osAisDataMap);
    void onDataReceived(QByteArray readDataAry);

    double getHeading();

    QGeoCoordinate getOsLoc();

    QString getGgaNmeaMsg();

    QString getGllNmeaMsg();

    QString getHdgNmeaMsg();
    void setOsDataSource(int value);

private slots: 

    QGeoCoordinate parseLocationFromNmea(QStringList listNmeaLocation);

private:

    QString baudRateReadGeneral;

    QGeoCoordinate gllLocation;

    QGeoCoordinate ggaLocation;

    int readGenBaudRate;

    double heading;

    QMutex mutex;

    QString gllNmeaMsg,ggaNmeaMsg,hdgNmeaMsg;
    QMap<QString,QString> nmeaDataMap;

    int osDataSource;

    NmeaToXml*_nmeaToXml;



};

#endif // READGENERAL_H
