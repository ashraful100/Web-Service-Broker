#ifndef SHSDATA_H
#define SHSDATA_H

#include <QObject>
#include <QMap>
#include <QGeoCoordinate>
#include <QTime>
#include <QtCore>

//#include <shsclient/cpacalculator.h>

//!
//! \brief The ShsData class handles/parses the incoming SHS data
//! \ingroup shsclient
//!
class ShsData : public QObject
{
    Q_OBJECT
public:
    explicit ShsData(QObject *parent = 0);


signals:
    void ShipVel(double value);
    void ShipHdg(double value);
    void ShipLat(double value);
    void ShipLon(double value);
    void osCurLoc(QGeoCoordinate cur_loc);



    void WindVel(double value);
    void WindDir(double value);

        // Waves

    void  SimulationTime(int);
    void SeaState(double value);
    void SeaStateDirection(double dir);
    void osRpm(double rpm);
    void osCog(double cog);
    void osSog(double sog);

    void ownShipData(double heading, QGeoCoordinate _locShipCent, double osLoa, double osBoa,QString shipName_str,int shipId);
    void commandCombinerEot(double cmdEot);

    void localTime(QTime localTime);
    void shipName(QString shipName);
    void shipNameWithId(QString shipName, int shipid);

    //!
    //! \brief Incoming OwnShip data object from ShsClient. This OwnShip data object is passed to other modules
    //! \param ownship map of OwnShip data
    //!
    void osObject(QMap<QString,QString> ownship);

    //!
    //! \brief Incoming TrafficShip data object from ShsClient. This TrafficShip data object is passed to other modules
    //! \param trafficship map of TrafficShip data
    //!
    void tsObject(QMap<QString,QString> trafficship);



public slots:

    void onShsRead(QMap<QString,QString> ShsData);
    void setShipId(int shipId);

    void setScenarioRandomiserState(bool state);
    void setVirtualShipName(QString &virtualName);
private slots:
    void setTrafShipData(QMap<QString, QString> ShsData, QMap<QString, QString> osMap);
    void setOwnShipData(QMap<QString, QString> ShsData);
private:

    int engineId;
    int rudderId;
    int thrusterId;
    int shipId;
    int dataInterval;
    int intervalcounter;
    bool scenarioRandomizerActive;

    QString virtualShipName;

    QMap<QString,QString> ownship;

    QMap<QString,QString> trafficship;
    //CpaCalculator _cpaCalc;

};

#endif // SHSDATA_H
